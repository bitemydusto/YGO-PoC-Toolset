#pragma once

#include <cstdint>
#include "Card.h"

namespace YGO
{
	enum class ZoneEffectType : uint8_t
	{
		Equip = 1,
		Absorb = 5,
		Permanent = 0xb,
		Temporary = 0xc,
	};
	class ZoneEffect
	{
	public:
		ZoneEffect(uint8_t side, uint8_t zone, uint8_t index);

		Card source();
		ZoneEffectType type();
		uint8_t param();
	private:
		uint8_t side;
		uint8_t zone;
		uint8_t index;
	};
}