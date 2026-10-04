#include "GOAT.h"

namespace
{
	constexpr uint16_t MONSTER_GATE = 0x802;

	uint32_t __cdecl Effect_MG(unsigned int* self, unsigned int* source, int mode);
	uint32_t __cdecl Condition_MG(unsigned int* self, unsigned int* source, int mode);
	uint32_t __cdecl Cost_MG(unsigned int* self, unsigned int* source, int mode);

	uint32_t __cdecl Effect_MG(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.finishedResolving) return 0;

		uint8_t state = GameData::GetEffectState();

		switch (state)
		{
			case 0x80:
			{
				if (duel->players[selfParam.playerIdx].cardsInDeck == 0) return 0;
				uint16_t cardIntID = duel->players[selfParam.playerIdx].deck[0].GetIntID();

				FUN::W_RevealTopCard(selfParam.playerIdx, FUN::GetCardID(cardIntID), 0);
				if (FUN::GetMonsterType(cardIntID) > 0x14 || FUN::HasInherentSummon(cardIntID) != 0 || FUN::CanBeSummonedByEffect(selfParam.playerIdx, cardIntID) == 0)
				{
					FUN::MillCards(selfParam.playerIdx, 1, 0);

					return 0x80;
				}
				return  0x7f;
			}
			case 0x7f:
			{
				auto& card = duel->players[selfParam.playerIdx].deck[0];

				FUN::SpecialSummon(selfParam.playerIdx, (uint32_t*)&card.fullValue, 1, 0x20, 0x0D, 0);
				return 0;
			}
		}

	}
	uint32_t __cdecl Condition_MG(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (duel->players[selfParam.playerIdx].cardsInDeck == 0) return 0;
		if (FUN::CanPlayerSummon(selfParam.playerIdx) == 0) return 0;
		if (FUN::NumOfEmptyValidSummonZones(selfParam.playerIdx) == 0) return 0;
		for (int i = 0; i < 5; i++)
		{
			auto card = duel->players[selfParam.playerIdx].cardZones[i].card;
			if (card.GetIntID() != 0) return 1;
		}

		return 0;
	}
	uint32_t __cdecl Cost_MG(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);
		
		auto sub = GameData::GetEffectSubState();

		switch (sub)
		{
			case 0:
			{
				FUN::ShowDialog("Select a monster to @3Tribute@0.");
				GameData::SetEffectSubState(1);
			}break;
			case 1:
			{
				if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

				uint8_t side = GameData::GetSelectedSide();
				uint8_t zone = GameData::GetSelectedZone();

				auto card = duel->players[side].cardZones[zone].card;
				if (side != selfParam.playerIdx || zone > 4 || card.GetIntID() == 0) return 0;

				if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

				FUN::MarkZoneAsTributed(side, zone);
				FUN::TributeSelected(side, zone);

				GameData::SetEffectSubState(0);
				return 1;
			}
		}

		return 0;
	}
}

void Install_MonsterGate()
{
	Register_EffectScript({
		.CardID = MONSTER_GATE,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_MG),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(&Condition_MG),
		.Cost = reinterpret_cast<uintptr_t>(&Cost_MG),
		.Target = 0
		});
}