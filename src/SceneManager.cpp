#include "AssetManager/SceneManager.h"

namespace AssetManager
{
	SceneManager::SceneManager(const std::string & assetPath, AssetManager<Mesh>* meshManager, AssetManager<Texture>* textureManager) :
		assetPath(assetPath),
		meshManager(meshManager),
		textureManager(textureManager)
	{
	}

	SceneManager::~SceneManager()
	{
	}

	Scene* SceneManager::Get(const std::string& name)
	{
		Scene* asset = nullptr;
		std::map<std::string, Scene*>::iterator it = assets.find(name);
		if (it != assets.end())
		{
			//Return the asset if it is already loaded
			asset = it->second;
		}
		else
		{
			//Load and return the asset if it is not already loaded
			std::ifstream inputFile(assetPath + name, std::ios::binary);
			if (inputFile.good())
			{
				//Extract only the filename after the forward slash
				std::size_t position = name.find_last_of('/');
				std::string filename;
				if (position == std::string::npos)
				{
					filename = name;
				}
				else
				{
					filename = name.substr(position + 1);
				}
				asset = assets[name] = new Scene(&inputFile, filename, meshManager, textureManager);
			}
			else
			{
				throw std::runtime_error("Failed to open asset: " + name);
			}
		}
		return asset;
	}
}