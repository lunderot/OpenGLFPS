#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "AssetManager/AssetManager.h"
#include "AssetManager/Audio.h"

class AudioPlayer
{
private:
	AssetManager::AssetManager<AssetManager::Audio> bufferManager;
	std::map<std::string, ALuint> sources;

	ALuint GetSource(const std::string& name);
	void Update(ALuint source, glm::vec3 pos, glm::vec3 vel = { 0, 0, 0 }, bool loop = false, float pitch = 1.0f, float gain = 1.0f);
public:
	AudioPlayer(const std::string& assetPath);
	~AudioPlayer();

	void Play(const std::string& name, const std::string& filename, glm::vec3 pos, glm::vec3 vel = { 0, 0, 0 }, bool loop = false, float pitch = 1.0f, float gain = 1.0f);
	bool IsPlaying(const std::string& name);
	void Update(const std::string& name, glm::vec3 pos, glm::vec3 vel = { 0, 0, 0 }, bool loop = false, float pitch = 1.0f, float gain = 1.0f);
};