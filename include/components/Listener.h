#pragma once
#include <kult.hpp>

namespace Component
{
	struct ListenerData
	{
		template<typename T> friend T&operator<<(T &os, const ListenerData &self) {
			return os << "listener", os;
		}
	};
	using Listener = kult::component<'list', ListenerData>;
}
