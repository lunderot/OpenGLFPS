#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/MenuRender.h"

#include "../AssetManager/Shader.h"

namespace Systems
{
	void MenuRender(AssetManager::Shader* shader, glm::ivec2 screenSize, bool debug);
}
