#pragma once

#include <cstdint>
#include "Card.h"

namespace YGO
{
	enum class ZoneEffectType : uint8_t
	{
		Equip = 0x1,
		Absorb = 0x5,
		Permanent = 0xb,
		Temporary = 0xc,
		WithCounter = 0xd,
	};

	class ZoneEffect
	{
	public:
		static void addToZone(uint8_t side, uint8_t zone);

		virtual ~ZoneEffect() = default;
	private:
		ZoneEffectType type;
	};

	class EquipEffect : public ZoneEffect
	{
	public:
		EquipEffect(Card source);
		Card equipCard();
	};

	class PermanentEffect : public ZoneEffect
	{
	public:
		PermanentEffect(Card* source);
		Card* source;
	};
}