#include "systems/AnimatedRender.h"

#include "components/Freelook.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Systems
{
	using namespace kult;

	void CalculateBones(Component::AnimatedRenderData& renderData, float deltaTime, std::vector<glm::mat4>& bones)
	{
		std::vector<AssetManager::Animation::Keyframe> prev, current, next;
		float prevTime, nextTime;
		auto keyframes = renderData.animation->GetKeyframes();
		renderData.time += deltaTime;
		if (renderData.time >= 1.0f)
		{
			renderData.time = 0.0f;
		}
		//Loop and interpolate keyframes
		for (auto it = keyframes.begin(); it != keyframes.end(); ++it)
		{
			if ((*it)[0].time > renderData.time)
			{
				next = *it;
				nextTime = (*it)[0].time;
				--it;
				prev = *it;
				prevTime = (*it)[0].time;
				break;
			}
		}
		//prev and next is now the keyframes to interpolate between
		current.resize(prev.size());
		float a = (renderData.time - prev[0].time) / (next[0].time - prev[0].time);
		
		for (int i = 0; i < current.size(); i++)
		{
			current[i].translation = glm::mix(prev[i].translation, next[i].translation, a);
			current[i].scale = glm::mix(prev[i].scale, next[0].scale, a);
			current[i].rotation = glm::slerp(prev[i].rotation, next[i].rotation, a);
		}

		for (int i = 0; i < current.size(); i++)
		{
			glm::mat4 bone;
			bone = glm::translate(bone, current[i].translation);
			bone *= glm::mat4_cast(current[i].rotation);
			bone = glm::scale(bone, current[i].scale);
			bones.push_back(bone);
		}

		auto hierarchy = renderData.animation->GetHierarchy();
		for (int i = 0; i < bones.size(); i++)
		{
			if (hierarchy[i] != -1)
			{
				bones[i] = bones[hierarchy[i]] * bones[i];
			}
		}
	}

	void AnimatedRender(AssetManager::Shader* shader, kult::entity camera, glm::ivec2 screenSize, glm::f32 fov, glm::f32 near, glm::f32 far, glm::f32 deltaTime)
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

		for (auto &id : join<Component::Position, Component::AnimatedRender>()) {
			auto& renderData = get<Component::AnimatedRender>(id);
			auto& positionData = get<Component::Position>(id);

			glm::mat4 model;
			model = glm::translate(model, positionData.pos);
			model *= glm::mat4_cast(positionData.rot);
			model = glm::scale(model, positionData.scale);


			std::vector<glm::mat4> tempAnimData;
			CalculateBones(renderData, deltaTime, tempAnimData);

			shader->SetUniform("model", model);
			shader->SetUniform("bone", *tempAnimData.data(), 4);


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderData.texture->GetTexture());

			glBindVertexArray(renderData.mesh->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, renderData.mesh->GetVertexCount());
		}
	}
}
