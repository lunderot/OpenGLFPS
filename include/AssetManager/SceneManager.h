#pragma once

#include <fstream>
#include <string>
#include <map>

#include "AssetManager/AssetManager.h"
#include "AssetManager/Scene.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/Texture.h"

namespace AssetManager
{
	class SceneManager
	{
	private:
		std::string assetPath;
		std::map<std::string, Scene*> assets;

		AssetManager<Mesh>* meshManager;
		AssetManager<Texture>* textureManager;
	public:
		SceneManager(const std::string& assetPath, AssetManager<Mesh>* meshManager, AssetManager<Texture>* textureManager);
		~SceneManager();

		Scene* Get(const std::string& name);
	};
}
