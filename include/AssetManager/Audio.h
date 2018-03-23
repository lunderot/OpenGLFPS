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
		Audio(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		~Audio();
		ALuint GetBuffer() const;

		void Load(unsigned char* data, size_t size, const std::string& filename, const void* userData);
		void Unload();
	};
}