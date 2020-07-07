#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>

namespace Component
{
	struct FreelookData
	{
		glm::f32 sensitivity;
		glm::vec2 rotation;

		template<typename T> friend T&operator<<(T &os, const FreelookData &self) {
			return os << "freelook", os;
		}
	};

	using Freelook = kult::component<0x6c6f6f6b /* look */, FreelookData>;
}
