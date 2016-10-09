#include "systems/Physics.h"

#include "glm/gtx/intersect.hpp"

namespace Systems
{
	using namespace kult;

	void Physics(float dt)
	{
		for (auto &id : join<Component::Position, Component::Physics>()) {
			auto& positionData = get<Component::Position>(id);
			auto& physicsData = get<Component::Physics>(id);

			physicsData.velocity += physicsData.acceleration * dt;
			positionData.pos += physicsData.velocity * dt;
			physicsData.rotationVelocity += physicsData.rotationAcceleration * dt;
			positionData.rot = glm::rotate(positionData.rot, physicsData.rotationVelocity.x * dt, glm::vec3(1, 0, 0));
			positionData.rot = glm::rotate(positionData.rot, physicsData.rotationVelocity.y * dt, glm::vec3(0, 1, 0));
			positionData.rot = glm::rotate(positionData.rot, physicsData.rotationVelocity.z * dt, glm::vec3(0, 0, 1));

			glm::f32 min = 100000.0f;
			glm::f32 playerEyeLevel = 1.7f;

			for (auto &id : join<Component::Collision>()) {
				auto& otherPositionData = get<Component::Position>(id);
				auto& collisionData = get<Component::Collision>(id);

				
				auto tri = collisionData.col->GetCollisionData();

				for (auto it = tri.begin(); it != tri.end(); it++)
				{
					glm::mat4 model;
					model = glm::translate(model, otherPositionData.pos);
					model *= glm::mat4_cast(otherPositionData.rot);
					model = glm::scale(model, otherPositionData.scale);

					glm::vec3 p1 = glm::vec3(model * glm::vec4(it->p[0], 1.0f));
					glm::vec3 p2 = glm::vec3(model * glm::vec4(it->p[1], 1.0f));
					glm::vec3 p3 = glm::vec3(model * glm::vec4(it->p[2], 1.0f));
					

					glm::vec3 baryPosition;
					if (glm::intersectRayTriangle<glm::vec3>(positionData.pos, glm::vec3(0, 0, -1), p1, p2, p3, baryPosition))
					{
						min = glm::min(baryPosition.z, min);
					}
				}
				
			}
			//std::cout << min << std::endl;
			if (min < playerEyeLevel)
			{
				positionData.pos.z += playerEyeLevel - min;
				physicsData.velocity.z = 0.0f;
				physicsData.acceleration.z = 0.0f;
			}
			else
			{
				physicsData.acceleration.z = -9.8f;
			}
		}

		
	}
}
