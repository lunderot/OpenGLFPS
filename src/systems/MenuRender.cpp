#include "systems/MenuRender.h"

#include "components/Freelook.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Systems
{
	using namespace kult;

	void MenuRender(AssetManager::Shader* shader, glm::ivec2 screenSize, bool debug)
	{

		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenSize.x), static_cast<float>(screenSize.y), 0.0f);

		shader->Use();
		shader->SetUniform("projview", projection);
		shader->SetUniform("tex", 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto &id : join<Component::Position, Component::MenuRender>()) {
			auto& renderData = get<Component::MenuRender>(id);
			auto& positionData = get<Component::Position>(id);

			glm::mat4 model;
			model = glm::translate(model, positionData.pos);
			model *= glm::mat4_cast(positionData.rot);
			model = glm::scale(model, positionData.scale);
			model = glm::scale(model, glm::vec3(1, -1, 1));

			shader->SetUniform("model", model);
			shader->SetUniform("scale", positionData.scale);
			shader->SetUniform("scaleuv", renderData.scaleUv);
			shader->SetUniform("debug", debug);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderData.texture->GetTexture());
			
			glBindVertexArray(renderData.mesh->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, renderData.mesh->GetVertexCount());
		}
	}
}
