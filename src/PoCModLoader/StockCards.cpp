#include "HookManager.h"
#include "GameData.h"
#include "Cards.h"

namespace
{
	auto* duel = GameData::GetDuel();

	void __stdcall LoadSelectionListFusion()
	{
		std::vector<uint32_t> fusions;
		GameData::Player player = duel->players[GameData::GetTurnPlayer()];

		for (size_t i = 0; i < player.cardsInExtra; i++)
		{
			auto card = player.extra[i];
			
			if (!HookManager::IsSynchroMonster(card.GetCardID()) && FUN::CanBeSummonedByEffect(GameData::GetTurnPlayer(), card.GetIntID()) != 0)
			{
				fusions.push_back(card.fullValue);
			}
		}
		GameData::ChangeSelectionList(fusions, 8);
	}

	uint32_t __cdecl M_Condition_Stein(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (duel->players[selfParam.playerIdx].lifePoints < 5001) return 0;
		if (duel->players[selfParam.playerIdx].cardsInExtra == 0) return 0;
		if (FUN::CanPlayerSummon(selfParam.playerIdx) == 0) return 0;
		if (FUN::NumOfEmptyValidSummonZones(selfParam.playerIdx) == 0) return 0;
		for (size_t i = 0; i < duel->players[selfParam.playerIdx].cardsInExtra; i++)
		{
			auto card = duel->players[selfParam.playerIdx].extra[i];
			if (!HookManager::IsSynchroMonster(card.GetCardID()) && FUN::CanBeSummonedByEffect(selfParam.playerIdx, card.GetIntID()) != 0)
			{
				return 1;
			}
		}

		return 0;
	}
}

void HookManager::PatchCards()
{
	//Cards::CYBER_STEIN
	Register_SelectionListPopulation(Cards::CYBER_STEIN, LoadSelectionListFusion);
	Register_EffectScript({
		.CardID = Cards::CYBER_STEIN,
		.Effect = 0x005853B0,
		.AppliesTo = 0,
		.Condition = (uintptr_t)M_Condition_Stein,
		.Cost = 0x0057C530,
		.Target = 0
		});
	//Cards::SUMMONER_OF_ILLUSIONS
	Register_SelectionListPopulation(Cards::SUMMONER_OF_ILLUSIONS, LoadSelectionListFusion);
}