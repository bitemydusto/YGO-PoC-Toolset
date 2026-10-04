#include "GOAT.h"

namespace
{
	constexpr uint16_t DARK_PALADIN = 0x804;
	uint32_t inst = 0;

	uint32_t __cdecl Effect_DarkPaladin(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.finishedResolving) return 0;
		if (inst != duel->players[selfParam.playerIdx].cardZones[selfParam.zoneIdx].card.GetInstance()) return 0;

		FUN::W_NegateActivation(source, true);

		return 0;
	}
	uint32_t __cdecl Condition_DarkPaladin(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (duel->players[selfParam.playerIdx].cardsInHand == 0) return 0;
		if (source == nullptr) return 0;
		if (mode != 0) return 0;

		FUN::Param sourceParam(source);
		uint16_t sourceIntID = sourceParam.cardIntID;
		if (FUN::GetMonsterType(sourceIntID) != CardType::Spell) return 0;

		if (sourceParam.zoneIdx < 5 || sourceParam.zoneIdx > 10) return 0;

		return 1;
	}
	uint32_t __cdecl Cost_DarkPaladin(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		FUN::SelectCardsToDiscard(selfParam.playerIdx, 1, 0, 0);
		inst = duel->players[selfParam.playerIdx].cardZones[selfParam.zoneIdx].card.GetInstance();

		return 1;
	}
	void __stdcall ChangeStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
	{
		int n = 0;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto card = duel->players[i].cardZones[j].card;
				if (card.GetIntID() != 0)
				{
					if (card.GetType() == CardType::Dragon) n++;
				}
			}
			for (int j = 0; j < duel->players[i].cardsInGrave; j++)
			{
				auto card = duel->players[i].grave[j];
				if (card.GetType() == CardType::Dragon) n++;
			}
		}

		// Modify stats
		// 0x20 = ATK, 0x24 = DEF
		uint32_t& refATK = *(uint32_t*)(statAddress + 0x20);

		refATK = refATK += 500 * n;
	}
}

void Install_DarkPaladin()
{
	Register_UnRevivable(DARK_PALADIN);
	Register_CanBeSummonedByEffect(DARK_PALADIN, false);
	Register_StatChange(DARK_PALADIN, ChangeStat);
	Register_ActivatableEffect(DARK_PALADIN);
	Register_SpellSpeed(DARK_PALADIN, 2);
	Register_Fusion2({
		.Result = DARK_PALADIN,
		.Materials = { Cards::BUSTER_BLADER, Cards::DARK_MAGICIAN }
		});

	Register_EffectScript({
		.CardID = DARK_PALADIN,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_DarkPaladin),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(&Condition_DarkPaladin),
		.Cost = reinterpret_cast<uintptr_t>(&Cost_DarkPaladin),
		.Target = 0
		});
}