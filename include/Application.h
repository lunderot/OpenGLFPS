#pragma once

#include "System.h"
#include "AssetManager/Shader.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/AnimatedMesh.h"
#include "AssetManager/Texture.h"
#include "AssetManager/Config.h"
#include "AssetManager/Scene.h"
#include "AssetManager/Audio.h"
#include "AssetManager/Animation.h"

#include "AudioPlayer.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <kult.hpp>

class Application: public System
{
private:
	AssetManager::AssetManager<AssetManager::Mesh> meshManager;
	AssetManager::AssetManager<AssetManager::AnimatedMesh> animatedMeshManager;
	AssetManager::AssetManager<AssetManager::Shader> shaderManager;
	AssetManager::AssetManager<AssetManager::Texture> textureManager;
	AssetManager::AssetManager<AssetManager::Config> configManager;
	AssetManager::Scene::UserData sceneManagerUserData;
	AssetManager::AssetManager<AssetManager::Scene, AssetManager::Scene::UserData> sceneManager;
	AssetManager::AssetManager<AssetManager::Animation> animationManager;
	AudioPlayer audioPlayer;

	AssetManager::Shader* shader;
	AssetManager::Shader* animatedShader;
	AssetManager::Shader* laserShader;

	kult::entity camera;
	kult::entity cube;
	kult::entity light;

	kult::entity animation;

	float dt;
	GLuint laservbo;
	GLuint laservao;
public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();

	template <unsigned numPoints>
	glm::vec3 GetBezierPoint(glm::vec3* points, float t)
	{
		glm::vec3 tmp[numPoints];
		memcpy(tmp, points, numPoints * sizeof(glm::vec3));
		int i = numPoints - 1;
		while (i > 0) {
			for (int k = 0; k < i; k++)
				tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
			i--;
		}
		return tmp[0];
	}
};

