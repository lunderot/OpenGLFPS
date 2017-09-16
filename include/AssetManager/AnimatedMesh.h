#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace AssetManager
{
	class AnimatedMesh
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
			glm::vec4 weight;
			glm::vec4 boneIndex;
		};
	private:
		GLuint vbo;
		GLuint vao;
		glm::u32 vertexCount;

		void LoadMesh(std::istream* buffer, std::vector<Vertex>& out);
	public:
		AnimatedMesh(std::istream* buffer, const std::string& filename, const void* userData);
		~AnimatedMesh();
		GLuint GetVBO() const;
		GLuint GetVAO() const;
		glm::u32 GetVertexCount() const;

		void Load(std::istream* buffer, const std::string& filename, const void* userData);
		void Unload();
	};
}
