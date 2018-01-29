#include "AssetManager/Shader.h"

#include <iostream>
#include <vector>
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

	void Shader::SetUniform(const GLchar* name, const glm::int32& scalar, unsigned int count)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform1iv(glGetUniformLocation(program, name), count, &scalar);
	}

	void Shader::SetUniform(const GLchar* name, const glm::float32& scalar, unsigned int count)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform1fv(glGetUniformLocation(program, name), count, &scalar);
	}

	void Shader::SetUniform(const GLchar* name, const glm::vec2& vector, unsigned int count)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform2fv(glGetUniformLocation(program, name), count, glm::value_ptr(vector));
	}

	void Shader::SetUniform(const GLchar* name, const glm::vec3& vector, unsigned int count)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform3fv(glGetUniformLocation(program, name), count, glm::value_ptr(vector));
	}

	void Shader::SetUniform(const GLchar* name, const glm::vec4& vector, unsigned int count)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniform4fv(glGetUniformLocation(program, name), count, glm::value_ptr(vector));
	}

	void Shader::SetUniform(const GLchar* name, const glm::mat4& matrix, unsigned int count)
	{
		SDL_assert(IsInUse() && "This shader does not match current shader on the GPU");
		glUniformMatrix4fv(glGetUniformLocation(program, name), count, GL_FALSE, glm::value_ptr(matrix));
	}

	GLuint Shader::CreateShader(const std::string& content, GLenum shaderType, const std::string& begin, const std::string& end)
	{
		size_t first, last;
		GLuint shader = 0;
		first = content.find(begin);
		last = content.find(end);

		if (first != std::string::npos && last != std::string::npos)
		{
			first += begin.size();

			shader = glCreateShader(shaderType);

			const GLchar* shaderSource[1] = { content.data() + first };
			GLint shaderSourceLength[1] = { last - first };

			glShaderSource(shader, 1, shaderSource, shaderSourceLength);
			glCompileShader(shader);

			CheckShaderError(shader, GL_COMPILE_STATUS, "Failed to compile shader");
		}
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
		std::string content((std::istreambuf_iterator<char>(*buffer)), (std::istreambuf_iterator<char>()));
		GLuint shader;
		std::vector<GLuint> shaders;

		program = glCreateProgram();
		
		shaders.push_back(shader = CreateShader(content, GL_VERTEX_SHADER, "<<<VS", "VS>>>"));
		if (shader != 0)
		{
			glAttachShader(program, shader);
		}
		shaders.push_back(shader = CreateShader(content, GL_FRAGMENT_SHADER, "<<<FS", "FS>>>"));
		if (shader != 0)
		{
			glAttachShader(program, shader);
		}

		shaders.push_back(shader = CreateShader(content, GL_GEOMETRY_SHADER, "<<<GS", "GS>>>"));
		if (shader != 0)
		{
			glAttachShader(program, shader);
		}

		glLinkProgram(program);
		CheckProgramError(program, GL_LINK_STATUS, "Failed to link program");

		glValidateProgram(program);
		CheckProgramError(program, GL_VALIDATE_STATUS, "Program is not valid");

		for (auto s: shaders)
		{
			glDetachShader(program, s);
			glDeleteShader(s);
		}
	}

	void Shader::Unload()
	{
		glDeleteProgram(program);
	}
}
