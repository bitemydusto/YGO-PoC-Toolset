#include "Action.h"
#include "../Addresses/Functions/Action.h"

namespace Engine::Functions::Action
{
	void PayLP(uint32_t playerIdx, uint32_t amount)
	{
		reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t)>(Addresses::Functions::Action::PAY_LP)(playerIdx, amount);
	}
	uint32_t SendCardFromField(Shared::Structures::EffectBlock* block, uint32_t zoneBitField, Shared::Types::Location destCode, uint32_t fxCode)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(Shared::Structures::EffectBlock*, uint32_t, Shared::Types::Location, uint32_t)>(Addresses::Functions::Action::SEND_CARD_FROM_FIELD)(block, zoneBitField, destCode, fxCode);
	}
	uint32_t DiscardFromHand(uint32_t playerIdx, uint32_t handIdx, uint32_t flag)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t)>(Addresses::Functions::Action::DISCARD_FROM_HAND)(playerIdx, handIdx, flag);
	}
	void DealEffectDamage(uint32_t playerIdx, uint32_t amount)
	{
		reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t)>(Addresses::Functions::Action::DEAL_EFFECT_DAMAGE)(playerIdx, amount);
	}
	void BanishFromGrave(uint32_t playerIdx, uint32_t* card, uint32_t flag)
	{
		reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t*, uint32_t)>(Addresses::Functions::Action::BANISH_FROM_GRAVE)(playerIdx, card, flag);
	}
	void Summon()
	{
		reinterpret_cast<void(__cdecl*)()>(Addresses::Functions::Action::SUMMON)();
	}
	void NormalSummon(uint32_t playerIdx, uint32_t handIdx, uint32_t destZone, uint32_t packedTributes, int set)
	{
		reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t, int)>(Addresses::Functions::Action::NORMAL_SUMMON)(playerIdx, handIdx, destZone, packedTributes, set);
	}
	void MarkZoneForTribute(uint32_t side, uint32_t zone)
	{
		reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t)>(Addresses::Functions::Action::MARK_ZONE_FOR_TRIBUTE)(side, zone);
	}
	void TributeZone(uint32_t side, uint32_t zone)
	{
		reinterpret_cast<void(__cdecl*)(uint32_t, uint32_t)>(Addresses::Functions::Action::TRIBUTE_ZONE)(side, zone);
	}
	void PayChainEnergy(uint32_t playerIdx)
	{
		reinterpret_cast<void(__cdecl*)(uint32_t)>(Addresses::Functions::Action::PAY_CHAIN_ENERGY)(playerIdx);
	}
}