#pragma once

#include <string>
#include <istream>

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

		template<typename T>
		T Get()
		{
			return *static_cast<T*>(&data[0]);
		}

		template<>
		std::string Get<std::string>()
		{
			return std::string(data);
		}

		template<>
		float Get<float>()
		{
			return floatValue;
		}

		template<>
		int Get<int>()
		{
			return integerValue;
		}

		void Load(std::istream* buffer, const std::string& filename, const void* userData);
	};
}
