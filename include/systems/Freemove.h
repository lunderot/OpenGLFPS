#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/Physics.h"
#include "components/Freemove.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif // _WIN32

namespace Systems
{
	void UpdateFreemove();
}
