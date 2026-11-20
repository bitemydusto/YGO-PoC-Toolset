#include "RandomCards.h"

namespace
{
	using Combination = GameData::Combination;

	constexpr uint16_t TRISHULA = 0x809;
	GameData::SynchroSummoner* summoner = nullptr;
	bool justSynchroSummoned = false;

	bool IsComboValid(Combination combo, GameData::SynchroSummoner* _summoner)
	{
		if (combo.GetCombinedLevel() != _summoner->level) return false;
		if (combo.GetNumOfTuners() != 1) return false;
		if ((combo.GetSize() - combo.GetNumOfTuners()) < 2) return false;

		return true;
	}
	bool CanBeSummoned(uint32_t playerIdx)
	{
		summoner = new GameData::SynchroSummoner(playerIdx, TRISHULA, IsComboValid);
		bool canBeSummoned = false;

		for (auto& combo : summoner->combinator.combinations)
		{
			if (combo.GetCombinedLevel() == summoner->level && combo.GetNumOfTuners() == 1 && (combo.GetSize() - combo.GetNumOfTuners()) > 1)
			{
				canBeSummoned = true;
				break;
			}
		}
		delete summoner;
		summoner = nullptr;

		return canBeSummoned;
	}
	uint32_t __stdcall SummonState()
	{
		if (summoner == nullptr) summoner = new GameData::SynchroSummoner(GameData::GetTurnPlayer(), TRISHULA, IsComboValid);

		uint32_t result = summoner->Standard_Synchro_SummonState();
		if (result == 1)
		{
			delete summoner;
			summoner = nullptr;
			justSynchroSummoned = true;
		}

		return result;
	}

	uint32_t __cdecl Effect_Trishula(EffectBlock* self, EffectBlock* source, int mode)
	{
		auto state = GameData::GetEffectState();

		switch (state)
		{
			case 0x80: // Hand
			{
				justSynchroSummoned = false;

				if (duel->players[self->GetSide() ^ 1].cardsInHand == 0) return 0x7e;
				FUN::W_ShowDialog("Do you want to banish a card from your opponent's hand?", DialogMode::YESNO, 0);
				return 0x7f;
			}
			case 0x7f:
			{
				if (GameData::GetDialogResult() != 0)
				{
					uint8_t handIndx = FUN::W_HighlightRandomCardInHand(self->GetSide() ^ 1, duel->players[self->GetSide() ^ 1].cardsInHand);
					FUN::W_MoveCard(duel->players[self->GetSide() ^ 1].hand[handIndx].fullValue, Location::HAND, Location::BANISHED);
				}
				return 0x7e;
			}
			case 0x7e: // Field
			{
				if (FUN::W_NumOfCardsOnField(self->GetSide() ^ 1) == 0) return 0x7c;
				FUN::W_ShowDialog("Do you want to banish a card from your opponent's field?", DialogMode::YESNO, 0);
				return 0x7d;
			}
			case 0x7d:
			{
				if (GameData::GetDialogResult() != 0)
				{
					return 0x6f;
				}
				return 0x7c;
			}
			case 0x7c: // Grave
			{
				if (duel->players[self->GetSide() ^ 1].cardsInGrave == 0) return 0x7a;
				FUN::W_ShowDialog("Do you want to banish a card from your opponent's graveyard?", DialogMode::YESNO, 0);
				return 0x7b;
			}
			case 0x7b:
			{
				if (GameData::GetDialogResult() != 0)
				{
					return 0x6e;
				}
				return 0x7a;
			}
			case 0x7a:
			{
				return 0;
			}
			case 0x6f: // Banish a card from opponent's field
			{
				uint8_t side = GameData::GetSelectedSide();
				uint8_t zone = GameData::GetSelectedColumn() + GameData::GetSelectedLocation();

				if (side == self->GetSide() || zone > 10) return 0x6f;

				uint16_t intID = duel->players[side].cardZones[zone].card.GetIntID();
				if (intID == 0) return 0x6f;

				if (FUN::IsFieldSelectionConfirmed() == 0) return 0x6f;

				FUN::FieldMaskGenerator maskGen;
				maskGen.zones[side][zone] = true;

				FUN::SendCardFromField((uint8_t*)self, maskGen.GenerateMask(), Location::BANISHED, 0);

				return 0x7c;
			}
			case 0x6e: // Banish a card from opponent's graveyard
			{
				FUN::InitiateSelectionList(self->GetSide(), 6, TRISHULA, 0);

				return 0x6d;
			}
			case 0x6d:
			{
				uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
				if (!entry || (*entry & 0xFFF) == 0) return 0x6d; // not ready

				FUN::W_MoveCard(*entry, Location::GRAVE, Location::BANISHED);

				return 0x7a;
			}
		}

		return 0;
	}
	uint32_t __cdecl Condition_Trishula(EffectBlock* self, EffectBlock* source, int mode)
	{
		return justSynchroSummoned ? 1 : 0;
	}
	void __stdcall SL_Trishula(uint32_t playerIdx)
	{
		std::vector<uint32_t> grave;
		auto& player = duel->players[GameData::GetTurnPlayer() ^ 1];

		for (size_t i = 0; i < player.cardsInGrave; i++)
		{
			auto& card = player.grave[i];

			grave.push_back(card.fullValue);
		}
		GameData::ChangeSelectionList(grave, 4);
	}
}

void Install_Trishula()
{
	Register_SynchroMonster(TRISHULA);
	Register_ExtraSummonMonster(TRISHULA, CanBeSummoned, SummonState);
	Register_SelectionListPopulation(TRISHULA, SL_Trishula);
	Register_SpecialSummonTrigger(TRISHULA);

	Register_EffectScript({
		.CardID = TRISHULA,
		.Effect = (uintptr_t)Effect_Trishula,
		.AppliesTo = 0,
		.Condition = (uintptr_t)Condition_Trishula,
		.Cost = 0,
		.Target = 0
		});
}