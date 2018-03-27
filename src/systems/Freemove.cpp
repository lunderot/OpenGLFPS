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
		for (auto &id : join<Component::Position, Component::Physics, Component::Freemove>())
		{
			auto& freemove = get<Component::Freemove>(id);
			glm::vec3 moveDirection(0, 0, 0);
			glm::f32 speedMultiplier = 1.0f;
			const glm::uint8* keyboardState = SDL_GetKeyboardState(nullptr);

			if (keyboardState[freemove.keys[0]])
			{
				moveDirection += glm::vec3(1, 0, 0);
			}
			if (keyboardState[freemove.keys[1]])
			{
				moveDirection += glm::vec3(-1, 0, 0);
			}
			if (keyboardState[freemove.keys[2]])
			{
				moveDirection += glm::vec3(0, 1, 0);
			}
			if (keyboardState[freemove.keys[3]])
			{
				moveDirection += glm::vec3(0, -1, 0);
			}
			if (keyboardState[freemove.keys[4]])
			{
				speedMultiplier = 2.0f;
			}

		
			if (glm::length2(moveDirection) > 0.0f)
			{
				moveDirection = normalize(moveDirection);
			}
			get<Component::Physics>(id).velocity = get<Component::Position>(id).rot * moveDirection * freemove.speed * speedMultiplier;

		}
	}
}
