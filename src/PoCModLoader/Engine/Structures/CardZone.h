#pragma once

#include <cstdint>
#include "Card.h"
#include "ZoneEffect.h"

namespace Structures
{
	struct CardZone
	{
		Card card;
		uint16_t unknown1;
		uint16_t status;
		uint16_t unknown2;
		uint16_t effectCount;
		uint16_t zoneEffectIDs[32];
		ZoneEffect zoneEffects[32];
		uint32_t stateFlags;

		bool IsFaceUp()
		{
			return (status & 2) != 0;
		}
		bool InAttackPosition()
		{
			return (status & 1) == 0;
		}
		uint8_t GetTurnCounter()
		{
			return (status & 0x3C) >> 2;
		}
		uint8_t GetDeathCounter()
		{
			return (status & 0x1C0) >> 6;
		}
	};
}