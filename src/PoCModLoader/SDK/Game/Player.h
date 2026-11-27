#pragma once

#include <cstdint>
#include "Card.h"
#include "Zone.h"

namespace YGO
{
	class Player
	{
	public:
		Player(uint8_t index);

		uint16_t LP();
		uint8_t handCount();
		Card* hand(uint8_t index);
		Zone* zones(uint8_t index);
	private:
		uint8_t playerIndex;
	};
}