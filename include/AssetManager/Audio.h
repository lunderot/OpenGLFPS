#pragma once
#include <string>
#include <fstream>
#include <vector>

#include "AL/al.h"
#include "AL/alc.h"

namespace AssetManager
{
	class Audio
	{
	private:
		ALuint buffer;
	public:
		Audio(std::istream* buffer, const std::string& filename, const void* userData);
		~Audio();
		ALuint GetBuffer() const;

		void Load(std::istream* buffer, const std::string& filename, const void* userData);
		void Unload();
	};
}