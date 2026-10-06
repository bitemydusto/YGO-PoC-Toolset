#include "RandomCards.h"

namespace
{
	constexpr uint16_t DDCROW = 0x807;

	uint32_t __cdecl Effect_DDCrow(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.finishedResolving) return 0;
		if (selfParam.targetCount == 0) return 0;

		FUN::W_MoveCard(selfParam.outerTargets[0], Location::GRAVE, Location::BANISHED);

		return 0;
	}
	uint32_t __cdecl Condition_DDCrow(EffectBlock* self, EffectBlock* source, int mode)
	{
		if (duel->players[0].cardsInGrave == 0 && duel->players[1].cardsInGrave == 0) return 0;

		return 1;
	}
	uint32_t __cdecl Cost_DDCrow(EffectBlock* self, EffectBlock* source, int mode)
	{
		uint16_t inst = self->GetInstance();
		uint8_t handIdx = FUN::GetInstIndexInHand(self->GetSide(), inst);

		if (handIdx > -1)
		{
			FUN::DiscardFromHand(self->GetSide(), handIdx, 0);
		}

		return 1;
	}
	uint32_t __cdecl Target_DDCrow(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		auto sub = GameData::GetEffectSubState();

		switch (sub)
		{
			case 0:
			{
				FUN::ShowDialog("Target a card in the graveyards.");

				GameData::SetEffectSubState(1);
			}break;
			case 1:
			{
				FUN::InitiateSelectionList(selfParam.playerIdx, 6, DDCROW, 0);

				GameData::SetEffectSubState(2);
			}break;
			case 2:
			{
				if (FUN::GetSelectionListCount() == 0)
				{
					GameData::SetEffectSubState(0);
					return 1;
				}

				uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
				if (!entry || (*entry & 0xFFF) == 0) return 0; // not ready

				FUN::W_HighlightAndStoreTarget(self, entry, Location::GRAVE);

				GameData::SetEffectSubState(0);
				return 1;
			}break;
		}

		return 0;
	}
	void __stdcall SelectionListPopulation_DDCrow(uint32_t playerIdx)
	{
		std::vector<uint32_t> items;
		for (uint8_t i = 0; i < duel->players[0].cardsInGrave; i++)
		{
			auto card = duel->players[0].grave[i];
			items.push_back(card.fullValue);
		}
		for (uint8_t i = 0; i < duel->players[1].cardsInGrave; i++)
		{
			auto card = duel->players[1].grave[i];
			items.push_back(card.fullValue);
		}
		GameData::ChangeSelectionList(items, 4);
	}
}

void Install_DDCrow()
{
	Register_HasEffectInHand(DDCROW);
	Register_SpellSpeed(DDCROW, 2);
	Register_SelectionListPopulation(DDCROW, SelectionListPopulation_DDCrow);
	Register_ResponseWindow(FUN::GetCardIntID(DDCROW), 0x3f);

	Register_EffectScript({
		.CardID = DDCROW,
		.Effect = (uintptr_t)Effect_DDCrow,
		.AppliesTo = 0,
		.Condition = (uintptr_t)Condition_DDCrow,
		.Cost = (uintptr_t)Cost_DDCrow,
		.Target = (uintptr_t)Target_DDCrow
		});
}