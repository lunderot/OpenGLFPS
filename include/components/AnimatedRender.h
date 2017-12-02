#pragma once
#include <kult.hpp>

#include "../AssetManager/AnimatedMesh.h"
#include "../AssetManager/Texture.h"
#include "../AssetManager/Animation.h"

namespace Component
{
	struct AnimatedRenderData
	{
		AssetManager::AnimatedMesh* mesh;
		AssetManager::Texture* texture;
		AssetManager::Animation* animation;
		float time;

		template<typename T> friend T&operator<<(T &os, const AnimatedRenderData &self) {
			return os << "animatedmesh", os;
		}
	};

	using AnimatedRender = kult::component<'arnd', AnimatedRenderData>;
}
