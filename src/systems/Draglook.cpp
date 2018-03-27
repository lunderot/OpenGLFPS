#include "systems/Draglook.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Systems
{
	using namespace kult;

	void HandleDraglookEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
		{
			for (auto &id : join<Component::Position, Component::Draglook>())
			{
				auto& cameraData = get<Component::Draglook>(id);
				if (event.motion.state & cameraData.buttonMask)
				{
					auto& positionData = get<Component::Position>(id);

					cameraData.rotation.x += (glm::f32)event.motion.xrel * cameraData.sensitivity * -1.0f;
					cameraData.rotation.y += (glm::f32)event.motion.yrel * cameraData.sensitivity;
					cameraData.rotation.y = glm::clamp(cameraData.rotation.y, -glm::half_pi<glm::f32>(), glm::half_pi<glm::f32>());
					positionData.rot = glm::rotate(glm::quat(), cameraData.rotation.x, glm::vec3(0, 0, 1));
					positionData.rot = glm::rotate(positionData.rot, cameraData.rotation.y, glm::vec3(0, 1, 0));

					glm::vec3 backward = glm::vec3(-1, 0, 0) * cameraData.offset;
					glm::vec3 offset = positionData.rot * backward;
					positionData.pos = cameraData.position + offset;
				}
			}
			break;
		}
		default:
			break;
		}
	}
}
