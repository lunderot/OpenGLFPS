#pragma once

#include <string>
#include <istream>
#include <cstring>
#include <stdexcept>

#include "SDL.h"

namespace AssetManager
{
	class Config
	{
	private:
		union
		{
			char stringValue[64];
			float floatValue;
			int integerValue;
			SDL_Scancode scancodeValue;
		};
	public:
		Config(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		~Config();

		std::string GetString()
		{
			return std::string(stringValue);
		}

		float GetFloat()
		{
			return floatValue;
		}

		int GetInt()
		{
			return integerValue;
		}

		SDL_Scancode GetScancode()
		{
			return scancodeValue;
		}

		void Load(unsigned char* data, size_t size, const std::string& filename, const void* userData);

		void Unload();
	};
}
