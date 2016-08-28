#include "systems/Render.h"

#include "components/Freelook.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Systems
{
	using namespace kult;

	void Render(AssetManager::Shader* shader, kult::entity camera, glm::ivec2 screenSize, glm::f32 fov, glm::f32 near, glm::f32 far)
	{
		auto& cameraPositionData = get<Component::Position>(camera);
		auto& cameraData = get<Component::Freelook>(camera);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (glm::f32)screenSize.x / screenSize.y, near, far);
		glm::vec3 lookDirection = cameraPositionData.rot * glm::vec3(1, 0, 0);
		glm::vec3 up = cameraPositionData.rot * glm::vec3(0, 0, 1);
		glm::mat4 view = glm::lookAt(cameraPositionData.pos, cameraPositionData.pos + lookDirection, up);

		shader->Use();
		shader->SetUniform("projview", projection * view);
		shader->SetUniform("tex", 0);

		shader->SetUniform("lightposition", glm::vec3(1, 0, 2));
		shader->SetUniform("lightcolor", glm::vec3(1, 1, 1));

		for (auto &id : join<Component::Position, Component::Render>()) {
			auto& renderData = get<Component::Render>(id);
			auto& positionData = get<Component::Position>(id);

			glm::mat4 model;
			model = glm::translate(model, positionData.pos);
			model *= glm::mat4_cast(positionData.rot);
			model = glm::scale(model, positionData.scale);

			shader->SetUniform("model", model);
			shader->SetUniform("scale", positionData.scale);
			shader->SetUniform("scaleuv", renderData.scaleUv);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderData.texture->GetTexture());
			
			glBindVertexArray(renderData.mesh->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, renderData.mesh->GetVertexCount());
		}
	}
}
