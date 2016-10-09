#include "AssetManager/Collision.h"
#include "AssetManager/Mesh.h"

namespace AssetManager
{
	Collision::Collision(std::istream * buffer, const std::string & filename, const void * userData)
	{
		Load(buffer, filename, userData);
	}

	Collision::~Collision()
	{
		Unload();
	}

	void Collision::Load(std::istream * buffer, const std::string & filename, const void * userData)
	{
		data.clear();
		std::vector<Mesh::Vertex> out;
		Mesh::LoadMesh(buffer, out);

		for (auto it = out.begin(); it != out.end(); it += 3)
		{
			data.push_back({ it->position, (it + 1)->position, (it + 2)->position });
		}
	}

	void Collision::Unload()
	{
	}

	const std::vector<Collision::Triangle>& Collision::GetCollisionData() const
	{
		return data;
	}
}