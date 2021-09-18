#include "systems/Freelook.h"

#include <glm/gtc/quaternion.hpp>

namespace Systems
{
	using namespace kult;

	void HandleFreelookEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			for (auto &id : join<Component::Position, Component::Freelook>()) {
				auto& positionData = get<Component::Position>(id);
				auto& cameraData = get<Component::Freelook>(id);

				cameraData.rotation.x += (glm::f32)event.motion.xrel * cameraData.sensitivity * -1.0f;
				cameraData.rotation.y += (glm::f32)event.motion.yrel * cameraData.sensitivity;
				cameraData.rotation.y = glm::clamp(cameraData.rotation.y, -glm::half_pi<glm::f32>(), glm::half_pi<glm::f32>());
				positionData.rot = glm::quat(glm::vec3(0, cameraData.rotation.y, cameraData.rotation.x));
			}
			break;
		default:
			break;
		}
	}
}
