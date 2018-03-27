#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Component
{
	struct DraglookData
	{
		glm::f32 sensitivity;
		glm::vec3 position;
		glm::f32 offset;
		glm::uint32 buttonMask;

		glm::vec2 rotation;
		bool mouseDown;
		

		template<typename T> friend T&operator<<(T &os, const DraglookData &self) {
			return os << "draglook", os;
		}
	};

	using Draglook = kult::component<'dlok', DraglookData>;
}
