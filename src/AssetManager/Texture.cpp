#include "AssetManager/Texture.h"

namespace AssetManager
{
	Texture::Texture(std::istream* buffer, const std::string& filename)
	{
		Load(buffer, filename);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &tex);
	}

	GLuint Texture::GetTexture() const
	{
		return tex;
	}

	void Texture::Load(std::istream* buffer, const std::string& filename)
	{
		std::vector<char> textureData;
		unsigned int width, height;
		size_t size;

		//Get the filesize in bytes
		buffer->seekg(0, buffer->end);
		size = static_cast<size_t>(buffer->tellg());
		buffer->seekg(0, buffer->beg);

		//Assume texture is always square and RGBA format
		width = height = static_cast<unsigned>(sqrt(size / 4));

		textureData.resize(size);
		//Read the whole file into the vector
		buffer->read((char*)textureData.data(), size);

		//Load the texture to the graphics card
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}
