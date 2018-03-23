#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <stdexcept>

#include "Util.h"

namespace AssetManager
{
	class Shader
	{
	private:
		GLuint program;

	private:
		GLuint CreateShader(const std::string& content, GLenum shaderType, const std::string& begin, const std::string& end);

		void CheckShaderError(GLuint shader, GLuint flag, const std::string& errorMessage);
		void CheckProgramError(GLuint program, GLuint flag, const std::string& errorMessage);

	public:
		Shader(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		~Shader();

		bool IsInUse() const;
		void Use() const;

		void SetUniform(const GLchar* name, const glm::int32& scalar, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::float32& scalar, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::vec2& vector, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::vec3& vector, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::vec4& vector, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::mat4& matrix, unsigned int count = 1);

		void Load(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		void Unload();
	};
}
