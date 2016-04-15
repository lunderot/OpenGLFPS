#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

namespace AssetManager
{
	class Texture
	{
	private:
		GLuint tex;
	public:
		Texture(std::istream* buffer, const std::string& filename);
		~Texture();

		void Load(std::istream* buffer, const std::string& filename);

		GLuint GetTexture() const;
	};
}
