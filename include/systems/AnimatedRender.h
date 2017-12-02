#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/AnimatedRender.h"
#include "components/Light.h"

#include "../AssetManager/Shader.h"

namespace Systems
{
	void AnimatedRender(AssetManager::Shader* shader, kult::entity camera, glm::ivec2 screenSize, glm::f32 fov, glm::f32 near, glm::f32 far, glm::f32 deltaTime);
}
