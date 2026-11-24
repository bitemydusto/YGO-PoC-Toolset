#pragma once

#include <cstdint>

namespace Engine::Types
{
	enum class SubType : uint8_t
	{
		NORMAL = 0,
		EFFECT = 1,
		FUSION = 2,
		RITUAL = 3,
	};
}
