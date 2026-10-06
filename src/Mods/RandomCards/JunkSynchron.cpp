#include "RandomCards.h"

namespace
{
	constexpr uint16_t JUNK_SYNCHRON = 0x805;

	uint32_t __cdecl Effect_JunkSynchron(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.finishedResolving) return 0;
		if (selfParam.targetCount == 0) return 0;

		auto tCard = GameData::Card { .fullValue = selfParam.outerTargets[0] };

		int index = FUN::GetInstIndexInGrave(selfParam.playerIdx, tCard.GetInstance());
		if (index < 0) return 0;

		FUN::SpecialSummon(selfParam.playerIdx, &(duel->players[selfParam.playerIdx].grave[index].fullValue), 1, 1, 0x20, 0xE, selfParam.playerIdx);

		return 0;
	}
	uint32_t __cdecl Condition_JunkSynchron(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.responseWindow != ResponseWindow::NORMAL_SUMMON) return 0;
		if (!FUN::W_CanSummon(selfParam.playerIdx)) return 0;
		if (duel->players[selfParam.playerIdx].cardsInGrave == 0) return 0;
		for (int i = 0; i < duel->players[selfParam.playerIdx].cardsInGrave; i++)
		{
			auto card = duel->players[selfParam.playerIdx].grave[i];
			if (card.GetType() < CardType::Trap && FUN::CanBeRevived(&card.fullValue))
			{
				if (card.GetLevel() < 3) return 1;
			}
		}

		return 0;
	}
	uint32_t __cdecl Target_JunkSynchron(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		auto sub = GameData::GetEffectSubState();

		switch (sub)
		{
			case 0:
			{
				FUN::ShowDialog("Do you want to @2Summon@0 a monster from your Graveyard to your graveyard?");
				FUN::ShowDialogOptions(1, 0);

				GameData::SetEffectSubState(1);
			}break;
			case 1:
			{
				if (GameData::GetDialogResult() == 0)
				{
					GameData::SetEffectSubState(0);
					return 1;
				}
				GameData::SetEffectSubState(2);
			}break;
			case 2:
			{
				*(uint16_t*)(selfParam.block + 4) &= 0x1FFF;
				FUN::ShowDialog("Select and monster in your graveyard to @2Summon@0.");

				GameData::SetEffectSubState(3);
			}break;
			case 3:
			{
				FUN::InitiateSelectionList(selfParam.playerIdx, 6, JUNK_SYNCHRON, 0);

				GameData::SetEffectSubState(4);
			}break;
			case 4:
			{
				uint32_t count = FUN::GetSelectionListCount();
				if (count == 0)
				{
					GameData::SetEffectSubState(0);
					return 1;
				}

				uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
				if (!entry || (*entry & 0xFFF) == 0) return 0; // not ready

				uint32_t dword = *entry;
				uint8_t  owner = (dword >> 12) & 1;
				uint32_t inst = owner + ((dword >> 24) & 0x7F) * 2;
				uint32_t sideBit = owner ? 0x8000u : 0;

				uint32_t cardId = FUN::GetCardID(dword & 0xFFF);

				// Highlight / reveal
				FUN::QueueCommand(sideBit | 0xDF, cardId, inst, 0);
				FUN::QueueCommand(sideBit | 0x08, owner, Location::GRAVE, 0);

				// Store targets
				FUN::StoreTarget((int)self, (uint16_t)dword);
				FUN::StoreTarget((int)self, (uint16_t)(dword >> 16));

				GameData::SetEffectSubState(0);
				return 1;
			}
		}

		return 0;
	}
	void __stdcall SL_JunkSynchron(uint32_t playerIdx)
	{
		std::vector<uint32_t> grave;
		auto& player = duel->players[GameData::GetTurnPlayer()];

		for (size_t i = 0; i < player.cardsInGrave; i++)
		{
			auto& card = player.grave[i];

			if (card.GetType() < CardType::Trap && FUN::CanBeRevived(&card.fullValue) && card.GetLevel() < 3)
			{
				grave.push_back(card.fullValue);
			}
		}
		GameData::ChangeSelectionList(grave, 4);
	}
}

void Install_JunkSynchron()
{
	Register_TunerMonster(JUNK_SYNCHRON);
	Register_NormalSummonTrigger(JUNK_SYNCHRON);
	Register_SelectionListPopulation(JUNK_SYNCHRON, SL_JunkSynchron);

	Register_EffectScript({
		.CardID = JUNK_SYNCHRON,
		.Effect = (uintptr_t)Effect_JunkSynchron,
		.AppliesTo = (uintptr_t)Condition_JunkSynchron,
		.Condition = (uintptr_t)Condition_JunkSynchron,
		.Cost = 0,
		.Target = (uintptr_t)Target_JunkSynchron
		});
}