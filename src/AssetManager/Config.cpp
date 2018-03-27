#include "AssetManager/Config.h"

#include <string>

namespace AssetManager
{
	Config::Config(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{
		Load(data, size, filename, userData);
	}

	Config::~Config()
	{
		Unload();
	}

	void Config::Load(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{
		std::string str(reinterpret_cast<char*>(data), size);

		switch (filename[0])
		{
		case 'i': //Integer
			integerValue = std::stoi(str);
			break;
		case 's': //String
			std::memset(stringValue, '\0', 64);
			std::memcpy(stringValue, str.data(), 64);
			break;
		case 'f': //Float
			floatValue = std::stof(str);
			break;
		case 'k': //Key
			scancodeValue = SDL_GetScancodeFromName(str.c_str());
			break;
		default:
			throw std::runtime_error("Invalid config variable type");
			break;
		}
	}

	void Config::Unload()
	{
	}
}
