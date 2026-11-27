#pragma once

#include <cstdint>
#include "../../Shared/Structures/EffectBlock.h"
#include "../../Shared/Types/Location.h"

namespace Engine::Functions::Action
{
	// Pays the specified amount of LP to the player
	void PayLP(uint32_t playerIdx, uint32_t amount);
	// Sends the card from the field to the destination based on the zone bit field (16 bit for each field, first 11 bits -> monster/st/fieldspell)
	// destCode: 0xb = hand, 0xd = deck, 0xe = grave, 0xf = banish
	// fxCode: bit field for effect, 0 = no effect, 2 = play destroy sound and visual effect
	uint32_t SendCardFromField(Shared::Structures::EffectBlock* block, uint32_t zoneBitField, Shared::Types::Location destCode, uint32_t fxCode);
	// Discards the card at the specified hand index for the specified player
	uint32_t DiscardFromHand(uint32_t playerIdx, uint32_t handIdx, uint32_t flag);
	// Deals the specified amount of effect damage to the player
	void DealEffectDamage(uint32_t playerIdx, uint32_t amount);
	// Banishes the specified card from the graveyard for the specified player
	void BanishFromGrave(uint32_t playerIdx, uint32_t* card, uint32_t flag);
	// Summons a monster using the pre-set summon parameters
	void Summon();
	// Normal summons/sets a monster from the hand of the specified player to the specified zone, consuming the packed tributes if given
	void NormalSummon(uint32_t playerIdx, uint32_t handIdx, uint32_t destZone, uint32_t packedTributes, int set);
	// Marks the specified zone for tribute with a visual effect
	void MarkZoneForTribute(uint32_t side, uint32_t zone);
	// Tributes the monster in the specified zone
	void TributeZone(uint32_t side, uint32_t zone);
	// Pays the cost of chain energy for the specified player
	void PayChainEnergy(uint32_t playerIdx);
}
