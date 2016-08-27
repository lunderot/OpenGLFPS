#pragma once

#include <string>
#include <istream>

#include "AssetManager/AssetManager.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/Texture.h"

#include "glm/glm.hpp"

#include "kult.hpp"

namespace AssetManager
{
	class Scene
	{
	private:
		std::vector<kult::entity> entities;
	public:
		Scene(std::istream* buffer, const std::string& filename, AssetManager<Mesh>* meshManager, AssetManager<Texture>* textureManager);
		~Scene();

		void Load(std::istream* buffer, const std::string& filename, AssetManager<Mesh>* meshManager, AssetManager<Texture>* textureManager);
	};
}
