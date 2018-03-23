#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace AssetManager
{
	class Texture
	{
	private:
		GLuint tex;
	public:
		Texture(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		~Texture();

		void Load(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		void Unload();

		GLuint GetTexture() const;
	};
}
