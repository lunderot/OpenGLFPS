#include "AssetManager/AnimatedMesh.h"

namespace AssetManager
{
	AnimatedMesh::AnimatedMesh(std::istream* buffer, const std::string& filename, const void* userData)
	{
		Load(buffer, filename, userData);
	}

	AnimatedMesh::~AnimatedMesh()
	{
		Unload();
	}

	void AnimatedMesh::LoadMesh(std::istream* buffer, std::vector<Vertex>& out)
	{
		std::vector<glm::u32> vertexIndices, uvIndices, normalIndices, boneIndices, weightIndices;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::uvec4> boneIndex;
		std::vector<glm::vec4> weights;


		glm::mat3 mat(1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 1.0f, 0.0f
			);

		while (!buffer->eof())
		{
			std::string command;
			*buffer >> command;
			if (command == "v")
			{
				glm::vec3 vertex;
				*buffer >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex*mat);
			}
			else if (command == "vt")
			{
				glm::vec2 uv;
				*buffer >> uv.x >> uv.y;
				uv.y *= -1.0f;
				uvs.push_back(uv);
			}
			else if (command == "vn")
			{
				glm::vec3 normal;
				*buffer >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal*mat);
			}
			else if (command == "w")
			{
				glm::vec4 w;
				*buffer >> w.x >> w.y >> w.z >> w.w;
				weights.push_back(w);
			}
			else if (command == "wi")
			{
				glm::uvec4 w;
				*buffer >> w.x >> w.y >> w.z >> w.w;
				boneIndex.push_back(w);
			}
			else if (command == "f")
			{
				glm::u32 vertexIndex[4], uvIndex[4], normalIndex[4], indexIndex[4], weightIndex[4];
				char padding;
				int count = 0;
				
				for (int i = 0; i < 3; i++)
				{
					*buffer >> vertexIndex[i] >> padding >> uvIndex[i] >> padding >> normalIndex[i] >> padding >> indexIndex[i] >> padding >> weightIndex[i];
					count++;
				}
				char next = (*buffer).peek();
				if (!(next == '\r' || next == '\n'))
				{
					*buffer >> vertexIndex[3] >> padding >> uvIndex[3] >> padding >> normalIndex[3] >> padding >> indexIndex[3] >> padding >> weightIndex[3];
					count++;
				}
				
				for (int i = 0; i < 3; i++)
				{
					vertexIndices.push_back(vertexIndex[i]);
					uvIndices.push_back(uvIndex[i]);
					normalIndices.push_back(normalIndex[i]);
					boneIndices.push_back(indexIndex[i]);
					weightIndices.push_back(weightIndex[i]);
				}
				if (count == 4)
				{
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[2]);
					vertexIndices.push_back(vertexIndex[3]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[2]);
					uvIndices.push_back(uvIndex[3]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[2]);
					normalIndices.push_back(normalIndex[3]);
					boneIndices.push_back(indexIndex[0]);
					boneIndices.push_back(indexIndex[2]);
					boneIndices.push_back(indexIndex[3]);
					weightIndices.push_back(weightIndex[0]);
					weightIndices.push_back(weightIndex[2]);
					weightIndices.push_back(weightIndex[3]);
				}
			}
			else //Unknown command
			{
				std::getline(*buffer, command);
			}
		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			Vertex vertex;
			vertex.position = vertices[vertexIndices[i] - 1];
			vertex.normal = normals[normalIndices[i] - 1];
			vertex.uv = uvs[uvIndices[i] - 1];
			vertex.boneIndex = boneIndex[boneIndices[i] - 1];
			vertex.weight = weights[weightIndices[i] - 1];
			out.push_back(vertex);
		}
	}

	GLuint AnimatedMesh::GetVBO() const
	{
		return vbo;
	}

	GLuint AnimatedMesh::GetVAO() const
	{
		return vao;
	}

	glm::u32 AnimatedMesh::GetVertexCount() const
	{
		return vertexCount;
	}

	void AnimatedMesh::Load(std::istream* buffer, const std::string& filename, const void* userData)
	{
		std::vector<Vertex> out;
		LoadMesh(buffer, out);

		//Generate vertex buffer and vertex array object
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);

		//Bind the buffers
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Load the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, out.size() * sizeof(Vertex), out.data(), GL_STATIC_DRAW);

		//Enable the shader attributes
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(6 * sizeof(float)));

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));



		glBindVertexArray(0);
		this->vertexCount = out.size();
	}

	void AnimatedMesh::Unload()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
}
