#pragma once

#include <cstdint>

namespace Engine::Types
{
	enum class CardTag : uint16_t
	{
		DESTROY = 0x0,
		SEARCH = 0x1,
		DRAW = 0x2,
		SUMMON_DECK = 0x3,
		SEND_FROM_DECK = 0x4
	};
}