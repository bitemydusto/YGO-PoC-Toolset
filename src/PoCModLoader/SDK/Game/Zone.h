#pragma once

#include <vector>
#include "Card.h"
#include "ZoneEffect.h"
#include "../../Shared/Types/Types.h"

namespace YGO
{
	class Zone
	{
		using Position = Shared::Types::Position;

	public:
		Zone(uint8_t side, uint8_t index);

		Card card();
		bool faceUp();
		Position position();
		uint32_t currentATK();
		uint32_t currentDEF();
		uint8_t turnCounters();
		uint8_t deathCounters();

		uint16_t effectCount();
		std::vector<ZoneEffect> effects();
		void addEffect(ZoneEffect* effect);
	private:
		uint8_t side;
		uint8_t index;
	};
}
