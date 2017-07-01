#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "AL/al.h"

#include "components/Listener.h"
#include "components/Position.h"
#include "components/Physics.h"



namespace Systems
{
	void UpdateListener();
}