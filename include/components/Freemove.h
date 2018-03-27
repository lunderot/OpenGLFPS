#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>

#include "SDL.h"

namespace Component
{
	struct FreemoveData
	{
		glm::f32 speed;
		SDL_Scancode keys[5];

		template<typename T> friend T&operator<<(T &os, const FreemoveData &self) {
			return os << "freemove", os;
		}
	};

	using Freemove = kult::component<'move', FreemoveData>;
}
