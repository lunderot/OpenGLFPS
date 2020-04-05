#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>

namespace Component
{
	struct LightData
	{
		glm::vec3 color;

		template<typename T> friend T&operator<<(T &os, const LightData &self) {
			return os << "lght", os;
		}
	};

	using Light = kult::component<0x6c676874 /* lght */, LightData>;
}
