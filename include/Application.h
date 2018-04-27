#pragma once

#include "System.h"
#include "AssetManager/Shader.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/Texture.h"
#include "AssetManager/Config.h"
#include "AssetManager/Scene.h"
#include "AssetManager/Audio.h"

#include "AudioPlayer.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

#include <kult.hpp>

#include <bitset>

class Application: public System
{
private:
	AssetManager::AssetManager<AssetManager::Mesh> meshManager;
	AssetManager::AssetManager<AssetManager::Shader> shaderManager;
	AssetManager::AssetManager<AssetManager::Texture> textureManager;
	AssetManager::AssetManager<AssetManager::Config> configManager;
	AssetManager::Scene::UserData sceneManagerUserData;
	AssetManager::AssetManager<AssetManager::Scene, AssetManager::Scene::UserData> sceneManager;
	AudioPlayer audioPlayer;
	AssetManager::Shader* shader;

	glm::f32 fov;
	glm::f32 near;
	glm::f32 far;

	kult::entity camera;
	kult::entity light;

	kult::entity marker[2];
	bool ghostid;
	kult::entity* ghost;

	std::bitset<9> board[2];
	int score[2];

	unsigned long mask[8];
private:
	//Game logic
	void PlacementLogic(SDL_Event & event);
	void CalculateScore();

	//Utility
	glm::vec3 GetRayFromMouse(glm::vec2 mouse, kult::entity camera);
	int WorldPositionToBoardPosition(glm::vec3 world);
public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();
};

