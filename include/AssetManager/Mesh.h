#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "AssetManager/Collision.h"

namespace AssetManager
{
	class Mesh
	{
		friend Collision;
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

		static void LoadMesh(std::istream* buffer, std::vector<Vertex>& out);
	public:
		Mesh(std::istream* buffer, const std::string& filename, const void* userData);
		~Mesh();
		GLuint GetVBO() const;
		GLuint GetVAO() const;
		glm::u32 GetVertexCount() const;

		void Load(std::istream* buffer, const std::string& filename, const void* userData);
		void Unload();
	};
}
