#pragma once

#include <cstdint>
#include "CardZone.h"
#include "Card.h"

namespace Structures
{
    struct Player
    {
        uint16_t lifePoints;
        uint8_t unkown1;
        uint8_t unkown2;
        uint8_t cardsInHand;
        uint8_t cardsInDeck;
        uint8_t cardsInGrave;
        uint8_t cardsInExtra;
        uint8_t cardsInBanish;
        uint8_t status;
        uint16_t canAttackZones;
        uint16_t alreadyAttackedZones;

        uint16_t padding;

        CardZone cardZones[11];
        CardZone playerZone;

        Card hand[80];
        Card deck[80];
        Card extra[15];
        Card grave[95];
        Card banish[95];

		uint8_t footer[0xc0]; // Padding for the rest of the player struct

        CardZone* monsterZone(uint8_t index)
        {
            if (index > 4) return &cardZones[4];
            return &cardZones[index];
        }
        CardZone* spellTrapZone(uint8_t index)
        {
            if (index < 5) return &cardZones[5];
            if (index > 9) return &cardZones[9];
            return &cardZones[index];
        }
        CardZone* fieldSpellZone()
        {
            return &cardZones[10];
        }
    };
}
