#include "AssetManager/Mesh.h"

namespace AssetManager
{
	Mesh::Mesh(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{
		Load(data, size, filename, userData);
	}

	Mesh::~Mesh()
	{
		Unload();
	}

	void Mesh::LoadMesh(unsigned char* data, size_t size, std::vector<Vertex>& out)
	{
		MemoryBuffer buf(data, size);
		std::istream buffer(&buf);

		std::vector<glm::u32> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		glm::mat3 mat(1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, -1.0f,
			0.0f, 1.0f, 0.0f
			);

		while (!buffer.eof())
		{
			std::string command;
			buffer >> command;
			if (command == "v")
			{
				glm::vec3 vertex;
				buffer >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex*mat);
			}
			else if (command == "vt")
			{
				glm::vec2 uv;
				buffer >> uv.x >> uv.y;
				uv.y *= -1.0f;
				uvs.push_back(uv);
			}
			else if (command == "vn")
			{
				glm::vec3 normal;
				buffer >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal*mat);
			}
			else if (command == "f")
			{
				glm::u32 vertexIndex[4], uvIndex[4], normalIndex[4];
				char padding;
				int count = 0;
				
				for (int i = 0; i < 3; i++)
				{
					buffer >> vertexIndex[i] >> padding >> uvIndex[i] >> padding >> normalIndex[i];
					count++;
				}
				char next = (buffer).peek();
				if (!(buffer.eof() || next == '\r' || next == '\n'))
				{
					buffer >> vertexIndex[3] >> padding >> uvIndex[3] >> padding >> normalIndex[3];
					count++;
				}
				
				for (int i = 0; i < 3; i++)
				{
					vertexIndices.push_back(vertexIndex[i]);
					uvIndices.push_back(uvIndex[i]);
					normalIndices.push_back(normalIndex[i]);
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
				}
			}
			else //Unknown command
			{
				std::getline(buffer, command);
			}
		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			Vertex vertex;
			vertex.position = vertices[vertexIndices[i] - 1];
			vertex.normal = normals[normalIndices[i] - 1];
			vertex.uv = uvs[uvIndices[i] - 1];
			out.push_back(vertex);
		}
	}

	GLuint Mesh::GetVBO() const
	{
		return vbo;
	}

	GLuint Mesh::GetVAO() const
	{
		return vao;
	}

	glm::u32 Mesh::GetVertexCount() const
	{
		return vertexCount;
	}

	void Mesh::Load(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{

		std::vector<Vertex> out;
		LoadMesh(data, size, out);

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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
		this->vertexCount = out.size();
	}

	void Mesh::Unload()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
}
