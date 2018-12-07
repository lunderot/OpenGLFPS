#pragma once
#include <kult.hpp>

#include "../AssetManager/Mesh.h"
#include "../AssetManager/Texture.h"

namespace Component
{
	struct MenuRenderData
	{
		AssetManager::Mesh* mesh;
		AssetManager::Texture* texture;
		bool scaleUv;
		template<typename T> friend T&operator<<(T &os, const MenuRenderData &self) {
			return os << "mesh", os;
		}
	};

	using MenuRender = kult::component<'mrnd', MenuRenderData>;
}
