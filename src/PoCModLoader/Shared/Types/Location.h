#pragma once

#include <cstdint>

namespace Shared::Types
{
	// 0-4: Monster Zones
	// 5-9: Spell/Trap Zones
	enum class Location : uint8_t
	{
		FIELDZONE = 0xA,
		HAND = 0xB,
		EXTRA = 0xC,
		DECK = 0xD,
		GRAVE = 0xE,
		BANISHED = 0xF
	};
}
