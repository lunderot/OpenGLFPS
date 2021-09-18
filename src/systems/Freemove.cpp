#include "systems/Freemove.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace Systems
{
	using namespace kult;

	void UpdateFreemove()
	{
		glm::vec3 moveDirection(0, 0, 0);
		glm::f32 speedMultiplier = 1.0f;
		const glm::uint8* keyboardState = SDL_GetKeyboardState(nullptr);

		if (keyboardState[SDL_SCANCODE_W])
		{
			moveDirection += glm::vec3(1, 0, 0);
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			moveDirection += glm::vec3(-1, 0, 0);
		}
		if (keyboardState[SDL_SCANCODE_A])
		{
			moveDirection += glm::vec3(0, 1, 0);
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			moveDirection += glm::vec3(0, -1, 0);
		}
		if (keyboardState[SDL_SCANCODE_LSHIFT])
		{
			speedMultiplier = 2.0f;
		}

		for (auto &id : join<Component::Position, Component::Physics, Component::Freemove>())
		{
			if (glm::length2(moveDirection) > 0.0f)
			{
				moveDirection = normalize(moveDirection);
			}
			get<Component::Physics>(id).velocity = get<Component::Position>(id).rot * moveDirection * get<Component::Freemove>(id).speed * speedMultiplier;
		}
	}
}
