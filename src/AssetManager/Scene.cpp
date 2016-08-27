#include "AssetManager/Scene.h"

#include "components/Position.h"
#include "components/Render.h"

namespace AssetManager
{
	Scene::Scene(std::istream * buffer, const std::string & filename, AssetManager<Mesh>* meshManager, AssetManager<Texture>* textureManager)
	{
		Load(buffer, filename, meshManager, textureManager);
	}

	Scene::~Scene()
	{
		for (auto e : entities)
		{
			e.purge();
		}
	}

	void Scene::Load(std::istream * buffer, const std::string & filename, AssetManager<Mesh>* meshManager, AssetManager<Texture>* textureManager)
	{
		while (!buffer->eof())
		{
			std::string filename, textureFilename;
			bool scaleUv;
			glm::vec3 pos;
			glm::vec3 scale;
			glm::vec3 rot;

			*buffer	>> filename
					>> textureFilename
					>> scaleUv
					>> pos.x >> pos.y >> pos.z
					>> rot.x >> rot.y >> rot.z
					>> scale.x >> scale.y >> scale.z;

			kult::entity e;
			kult::add<Component::Position>(e) = {
				pos,
				glm::quat(rot),
				scale
			};
			kult::add<Component::Render>(e) = {
				meshManager->Get(filename),
				textureManager->Get(textureFilename),
				scaleUv
			};
			entities.push_back(e);
		}
	}
}