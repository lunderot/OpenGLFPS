#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Util.h"

namespace AssetManager
{
	class Mesh
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};
	private:
		GLuint vbo;
		GLuint vao;
		glm::u32 vertexCount;

		void LoadMesh(unsigned char* data, size_t size, std::vector<Vertex>& out);
	public:
		Mesh(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		~Mesh();
		GLuint GetVBO() const;
		GLuint GetVAO() const;
		glm::u32 GetVertexCount() const;

		void Load(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		void Unload();
	};
}
