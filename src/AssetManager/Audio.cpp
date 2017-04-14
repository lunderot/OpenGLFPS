#include "AssetManager/Audio.h"

#include "stb_vorbis.c"

namespace AssetManager
{
	Audio::Audio(std::istream* buffer, const std::string& filename, const void* userData)
	{
		Load(buffer, filename, userData);
	}

	Audio::~Audio()
	{
		Unload();
	}

	ALuint Audio::GetBuffer() const
	{
		return buffer;
	}

	void Audio::Load(std::istream* buffer, const std::string& filename, const void* userData)
	{
		std::string str((std::istreambuf_iterator<char>(*buffer)), std::istreambuf_iterator<char>());
		int channels;
		int sampleRate;
		short* audioBuffer;
		int dataLength;
		dataLength = stb_vorbis_decode_memory((const uint8*)str.data(), str.size(), &channels, &sampleRate, &audioBuffer);

		alGenBuffers(1, &this->buffer);
		alBufferData(this->buffer, AL_FORMAT_MONO16, audioBuffer, dataLength * sizeof(ALshort), sampleRate);
	}

	void Audio::Unload()
	{
		alDeleteBuffers(1, &buffer);
	}

}