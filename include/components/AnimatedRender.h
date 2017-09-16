#pragma once
#include <kult.hpp>

#include "../AssetManager/AnimatedMesh.h"
#include "../AssetManager/Texture.h"

namespace Component
{
	struct AnimatedRenderData
	{
		AssetManager::AnimatedMesh* mesh;
		AssetManager::Texture* texture;

		template<typename T> friend T&operator<<(T &os, const AnimatedRenderData &self) {
			return os << "animatedmesh", os;
		}
	};

	using AnimatedRender = kult::component<'arnd', AnimatedRenderData>;
}
