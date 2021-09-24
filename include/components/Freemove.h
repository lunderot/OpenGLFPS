#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>

namespace Component
{
	struct FreemoveData
	{
		glm::f32 speed;

		template<typename T> friend T&operator<<(T &os, const FreemoveData &self) {
			return os << "freemove", os;
		}
	};

	using Freemove = kult::component<0x6d6f7665 /* move */, FreemoveData>;
}
