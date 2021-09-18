#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(const std::string& assetPath) :
	bufferManager(assetPath)
{
}

AudioPlayer::~AudioPlayer()
{
}

ALuint AudioPlayer::GetSource(const std::string& name)
{
	std::map<std::string, ALuint>::iterator it = sources.find(name);
	if (it == sources.end())
	{
		alGenSources((ALuint)1, &sources[name]);
		alGetError();
		auto error = alGetError();
		if (error != AL_NO_ERROR)
		{
			throw std::runtime_error("Error generating OpenAL source: " + error);
		}
	}
	return sources[name];
}

void AudioPlayer::Play(const std::string& name, const std::string& filename, glm::vec3 pos, glm::vec3 vel, bool loop, float pitch, float gain)
{
	ALuint source = GetSource(name);
	Update(source, pos, vel, loop, pitch, gain);
	alSourcei(source, AL_BUFFER, bufferManager.Get(filename)->GetBuffer());
	alSourcePlay(source);
}

void AudioPlayer::Update(ALuint source, glm::vec3 pos, glm::vec3 vel, bool loop, float pitch, float gain)
{
	alSourcefv(source, AL_POSITION, glm::value_ptr(pos));
	alSourcefv(source, AL_VELOCITY, glm::value_ptr(vel));
	alSourcei(source, AL_LOOPING, loop);
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
}

void AudioPlayer::Update(const std::string& name, glm::vec3 pos, glm::vec3 vel, bool loop, float pitch, float gain)
{
	ALuint source = GetSource(name);
	Update(source, pos, vel, loop, pitch, gain);
}