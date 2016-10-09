#pragma once
#include <kult.hpp>

#include "../AssetManager/Collision.h"

namespace Component
{
	struct CollisionData
	{
		AssetManager::Collision* col;
		template<typename T> friend T&operator<<(T &os, const CollisionData &self) {
			return os << "coli", os;
		}
	};

	using Collision = kult::component<'coli', CollisionData>;
}

