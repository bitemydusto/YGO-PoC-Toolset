#pragma once

#include <cstdint>

namespace Shared::Types
{
	enum class CardSubType : uint8_t
	{
		NORMAL = 0,
		EFFECT = 1,
		FUSION = 2,
		RITUAL = 3,
	};
}
