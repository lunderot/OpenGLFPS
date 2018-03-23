#include "AssetManager/Texture.h"

namespace AssetManager
{
	Texture::Texture(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{
		Load(data, size, filename, userData);
	}

	Texture::~Texture()
	{
		Unload();
	}

	GLuint Texture::GetTexture() const
	{
		return tex;
	}

	void Texture::Load(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{
		unsigned int width, height;

		//Assume texture is always square and RGBA format
		width = height = static_cast<unsigned>(sqrt(size / 4));

		//Load the texture to the graphics card
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture::Unload()
	{
		glDeleteTextures(1, &tex);
	}
}
