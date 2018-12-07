#include "AssetManager/Audio.h"

#include "stb_vorbis.c"

namespace AssetManager
{
	Audio::Audio(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{
		Load(data, size, filename, userData);
	}

	Audio::~Audio()
	{
		Unload();
	}

	ALuint Audio::GetBuffer() const
	{
		return buffer;
	}

	void Audio::Load(unsigned char* data, size_t size, const std::string& filename, const void* userData)
	{
		int channels;
		int sampleRate;
		short* audioBuffer;
		int dataLength;
		dataLength = stb_vorbis_decode_memory(static_cast<const uint8*>(data), static_cast<int>(size), &channels, &sampleRate, &audioBuffer);

		alGenBuffers(1, &buffer);
		alBufferData(buffer, AL_FORMAT_MONO16, audioBuffer, dataLength * sizeof(ALshort), sampleRate);
	}

	void Audio::Unload()
	{
		alDeleteBuffers(1, &buffer);
	}

}