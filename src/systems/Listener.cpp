#include "systems/Listener.h"

namespace Systems
{
	using namespace kult;

	void UpdateListener()
	{
		for (auto &id : kult::join<Component::Listener, Component::Physics>())
		{
			auto positionData = get<Component::Position>(id);
			auto physicsData = get<Component::Physics>(id);

			alListenerfv(AL_POSITION, glm::value_ptr(positionData.pos));
			alListenerfv(AL_VELOCITY, glm::value_ptr(physicsData.velocity));

			glm::vec3 dir = positionData.rot * glm::vec3(1, 0, 0);
			ALfloat listenerOri[] = { dir.x, dir.y, dir.z, 0.0f, 0.0f, 1.0f };

			alListenerfv(AL_ORIENTATION, listenerOri);
		}
	}
}
