#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>

namespace Component
{
	struct PhysicsData
	{
		glm::vec3 velocity = glm::vec3(0, 0, 0);
		glm::vec3 acceleration = glm::vec3(0, 0, 0);
		glm::vec3 rotationVelocity = glm::vec3(0, 0, 0);
		glm::vec3 rotationAcceleration = glm::vec3(0, 0, 0);

		template<typename T> friend T&operator<<(T &os, const PhysicsData &self) {
			return os << "(x:" << self.velocity.x << ",y:" << self.velocity.y << ",z:" << self.velocity.z << ")", os;
		}
	};

	using Physics = kult::component<0x706879 /* phy */, PhysicsData>;
}
