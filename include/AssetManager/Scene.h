#pragma once

#include <string>
#include <istream>

#include "AssetManager/AssetManager.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/Texture.h"
#include "AssetManager/Collision.h"

#include "glm/glm.hpp"

#include "kult.hpp"

namespace AssetManager
{
	class Scene
	{
	public:
		struct UserData
		{
			AssetManager<Mesh>* meshManager;
			AssetManager<Texture>* textureManager;
			AssetManager<Collision>* collisionManager;
		};
	private:
		std::vector<kult::entity> entities;
	public:
		Scene(std::istream* buffer, const std::string& filename, const UserData* userData);
		~Scene();

		void Load(std::istream* buffer, const std::string& filename, const UserData* userData);
		void Unload();
	};
}
