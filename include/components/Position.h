#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Component
{
	struct PositionData
	{
		glm::vec3 pos = glm::vec3(0, 0, 0);
		glm::quat rot = glm::quat(1, 0, 0, 0);
		glm::vec3 scale = glm::vec3(1, 1, 1);

		template<typename T> friend T&operator<<(T &os, const PositionData &self) {
			return os << "(x:" << self.pos.x << ",y:" << self.pos.y << ",z:" << self.pos.z << ",rx:" << self.rot.x << ",ry : " << self.rot.y << ",rz : " << self.rot.z << ")", os;
		}
	};

	using Position = kult::component<0x706f73 /* pos */, PositionData>;
}
