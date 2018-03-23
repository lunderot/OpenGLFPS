#include "AssetManager/Scene.h"

#include "components/Position.h"
#include "components/Render.h"

namespace AssetManager
{
	Scene::Scene(unsigned char* data, size_t size, const std::string & filename, const UserData* userData)
	{
		Load(data, size, filename, userData);
	}

	Scene::~Scene()
	{
		Unload();
	}

	void Scene::Load(unsigned char* data, size_t size, const std::string & filename, const UserData* userData)
	{
		MemoryBuffer buf(data, size);
		std::istream buffer(&buf);

		while (!buffer.eof())
		{
			std::string filename, textureFilename;
			bool scaleUv;
			glm::vec3 pos;
			glm::vec3 scale;
			glm::vec3 rot;

			buffer	>> filename
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
				userData->meshManager->Get(filename),
				userData->textureManager->Get(textureFilename),
				scaleUv
			};
			entities.push_back(e);
		}
	}

	void Scene::Unload()
	{
		for (auto e : entities)
		{
			e.purge();
		}
	}
}