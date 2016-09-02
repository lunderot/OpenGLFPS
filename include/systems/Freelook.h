#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/Physics.h"
#include "components/Freelook.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif // _WIN32

namespace Systems
{
	void HandleFreelookEvent(SDL_Event& event);
}
