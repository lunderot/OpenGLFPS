#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Util.h"

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

		void LoadMesh(unsigned char* data, size_t size, std::vector<Vertex>& out);
	public:
		AnimatedMesh(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		~AnimatedMesh();
		GLuint GetVBO() const;
		GLuint GetVAO() const;
		glm::u32 GetVertexCount() const;

		void Load(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		void Unload();
	};
}
