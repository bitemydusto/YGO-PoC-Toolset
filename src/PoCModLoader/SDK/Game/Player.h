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
		uint8_t cardsInHand();
		uint8_t cardsInDeck();
		uint8_t cardsInGrave();
		uint8_t cardsInExtra();
		uint8_t cardsInBanish();

		Zone zone(uint8_t index);
		Zone monsterZone(uint8_t index);
		Zone spellTrapZone(uint8_t index);
		Zone fieldSpellZone();

		Card hand(uint8_t index);
		Card deck(uint8_t index);
		Card grave(uint8_t index);
		Card extra(uint8_t index);
		Card banish(uint8_t index);
	private:
		uint8_t playerIndex;
	};
}