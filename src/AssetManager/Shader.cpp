#include "AssetManager/Shader.h"

#include <iostream>
#ifdef _WIN32
	#include <SDL_assert.h>
#else
	#include <SDL2/SDL_assert.h>
#endif // _WIN32

namespace AssetManager
{
	Shader::Shader(std::istream* buffer, const std::string& filename, const void* userData)
	{
		Load(buffer, filename, userData);
	}

	Shader::~Shader()
	{
		Unload();
	}

	bool Shader::IsInUse() const
	{
		GLint currentProgram = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

		return currentProgram == (GLint)program;
	}

	void Shader::Use() const
	{
		glUseProgram(program);
	}

	void Shader::SetUniform(const GLchar* name, const glm::int32& scalar)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform1i(glGetUniformLocation(program, name), scalar);
	}

	void Shader::SetUniform(const GLchar* name, const glm::float32& scalar)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform1f(glGetUniformLocation(program, name), scalar);
	}

	void Shader::SetUniform(const GLchar* name, const glm::vec2& vector)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform2fv(glGetUniformLocation(program, name), 1, glm::value_ptr(vector));
	}

	void Shader::SetUniform(const GLchar* name, const glm::vec3& vector)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(vector));
	}

	void Shader::SetUniform(const GLchar* name, const glm::vec4& vector)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(vector));
	}

	void Shader::SetUniform(const GLchar* name, const glm::mat4& matrix)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	GLuint Shader::CreateShader(const std::string& content, GLenum shaderType)
	{
		GLuint shader = glCreateShader(shaderType);

		const GLchar* shaderSource[1];
		GLint shaderSourceLength[1];
		shaderSource[0] = content.c_str();
		shaderSourceLength[0] = content.length();

		glShaderSource(shader, 1, shaderSource, shaderSourceLength);
		glCompileShader(shader);

		CheckShaderError(shader, GL_COMPILE_STATUS, "Failed to compile shader");

		return shader;
	}

	void Shader::CheckShaderError(GLuint shader, GLuint flag, const std::string& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = { 0 };

		glGetShaderiv(shader, flag, &success);

		if (success == GL_FALSE)
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
			throw std::runtime_error(errorMessage + ": " + error);
		}
	}

	void Shader::CheckProgramError(GLuint program, GLuint flag, const std::string& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = { 0 };

		glGetProgramiv(program, flag, &success);

		if (success == GL_FALSE)
		{
			glGetProgramInfoLog(program, sizeof(error), NULL, error);
			throw std::runtime_error(errorMessage + ": " + error);
		}
	}

	void Shader::Load(std::istream* buffer, const std::string& filename, const void* userData)
	{
		unsigned first, last;

		std::string content((std::istreambuf_iterator<char>(*buffer)), (std::istreambuf_iterator<char>()));

		first = content.find("<<<VS") + 5;
		last = content.find("VS>>>");
		shader[0] = CreateShader(content.substr(first, last - first), GL_VERTEX_SHADER);

		first = content.find("<<<FS") + 5;
		last = content.find("FS>>>");
		shader[1] = CreateShader(content.substr(first, last - first), GL_FRAGMENT_SHADER);

		program = glCreateProgram();
		for (int i = 0; i < NUM_SHADERS; i++)
		{
			glAttachShader(program, shader[i]);
		}

		glLinkProgram(program);
		CheckProgramError(program, GL_LINK_STATUS, "Failed to link program");

		glValidateProgram(program);
		CheckProgramError(program, GL_VALIDATE_STATUS, "Program is not valid");
	}

	void Shader::Unload()
	{
		for (int i = 0; i < NUM_SHADERS; i++)
		{
			glDetachShader(program, shader[i]);
			glDeleteShader(shader[i]);
		}
		glDeleteProgram(program);
	}
}
