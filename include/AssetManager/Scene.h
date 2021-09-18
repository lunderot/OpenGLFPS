#pragma once

#include <string>
#include <istream>

#include "AssetManager/AssetManager.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/Texture.h"

#include "glm/glm.hpp"

#include "kult.hpp"

#include "Util.h"

namespace AssetManager
{
	class Scene
	{
	public:
		struct UserData
		{
			AssetManager<Mesh>* meshManager;
			AssetManager<Texture>* textureManager;
		};
	private:
		std::vector<kult::entity> entities;
	public:
		Scene(unsigned char* data, size_t size, const std::string& filename, const UserData* userData);
		~Scene();

		void Load(unsigned char* data, size_t size, const std::string& filename, const UserData* userData);
		void Unload();
	};
}
