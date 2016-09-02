#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <stdexcept>

namespace AssetManager
{
	class Shader
	{
	private:
		static const unsigned int NUM_SHADERS = 2;
		GLuint program;
		GLuint shader[NUM_SHADERS];

	private:
		GLuint CreateShader(const std::string& content, GLenum shaderType);

		void CheckShaderError(GLuint shader, GLuint flag, const std::string& errorMessage);
		void CheckProgramError(GLuint program, GLuint flag, const std::string& errorMessage);

	public:
		Shader(std::istream* buffer, const std::string& filename, const void* userData);
		~Shader();

		bool IsInUse() const;
		void Use() const;

		void SetUniform(const GLchar* name, const glm::int32& scalar, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::float32& scalar, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::vec2& vector, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::vec3& vector, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::vec4& vector, unsigned int count = 1);
		void SetUniform(const GLchar* name, const glm::mat4& matrix, unsigned int count = 1);

		void Load(std::istream* buffer, const std::string& filename, const void* userData);
		void Unload();
	};
}
