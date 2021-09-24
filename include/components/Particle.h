#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>

namespace Component
{
	struct ParticleData
	{
		enum class Type
		{
			NONE,
			LASER,
		} type;

		template<typename T> friend T&operator<<(T &os, const ParticleData&self) {
			return os << "particle", os;
		}
	};

	using Particle = kult::component<0x70617274  /* part */, ParticleData>;
}
