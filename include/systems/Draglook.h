#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/Physics.h"
#include "components/Draglook.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif // _WIN32

namespace Systems
{
	void HandleDraglookEvent(SDL_Event& event);
}
