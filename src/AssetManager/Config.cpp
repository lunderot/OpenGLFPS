#include "AssetManager/Config.h"

#include <string>

namespace AssetManager
{
	Config::Config(std::istream* buffer, const std::string& filename)
	{
		Load(buffer, filename);
	}

	Config::~Config()
	{
	}

	void Config::Load(std::istream* buffer, const std::string& filename)
	{
		std::string str((std::istreambuf_iterator<char>(*buffer)), std::istreambuf_iterator<char>());
		switch (filename[0])
		{
		case 'i':
			integerValue = std::stoi(str);
			break;
		case 's':
			std::memset(data, '\0', 64);
			std::memcpy(data, str.data(), 64);
			break;
		case 'f':
			floatValue = std::stof(str);
			break;
		default:
			throw std::runtime_error("Invalid config variable type");
			break;
		}
	}
}
