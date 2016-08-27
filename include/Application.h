#pragma once

#include "System.h"
#include "AssetManager/Shader.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/Texture.h"
#include "AssetManager/Config.h"

#include "AssetManager/SceneManager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <kult.hpp>

class Application: public System
{
private:
	AssetManager::AssetManager<AssetManager::Mesh> meshManager;
	AssetManager::AssetManager<AssetManager::Shader> shaderManager;
	AssetManager::AssetManager<AssetManager::Texture> textureManager;
	AssetManager::AssetManager<AssetManager::Config> configManager;

	AssetManager::Shader* shader;

	AssetManager::SceneManager sceneManager;

	kult::entity camera;
public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();
};

