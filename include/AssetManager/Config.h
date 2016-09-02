#pragma once

#include <string>
#include <istream>
#include <cstring>
#include <stdexcept>

namespace AssetManager
{
	class Config
	{
	private:
		union
		{
			char data[64];
			float floatValue;
			int integerValue;
		};
	public:
		Config(std::istream* buffer, const std::string& filename, const void* userData);
		~Config();

		std::string GetString()
		{
			return std::string(data);
		}

		float GetFloat()
		{
			return floatValue;
		}

		int GetInt()
		{
			return integerValue;
		}

		void Load(std::istream* buffer, const std::string& filename, const void* userData);

		void Unload();
	};
}
