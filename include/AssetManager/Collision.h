#pragma once
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AssetManager
{
	class Collision
	{
	public:
		struct Triangle
		{
			glm::vec3 p[3];
		};
	private:
		std::vector<Triangle> data;
	public:
		Collision(std::istream* buffer, const std::string& filename, const void* userData);
		~Collision();

		void Load(std::istream* buffer, const std::string& filename, const void* userData);
		void Unload();

		const std::vector<Triangle>& GetCollisionData() const;
	};
}
#pragma once
