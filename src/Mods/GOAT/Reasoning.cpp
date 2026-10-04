#include "GOAT.h"

namespace
{
	constexpr uint16_t REASONING = 0x801;
	constexpr uint16_t REASONING_PROXY = Cards::RUDE_KAISER;

	uint8_t reasoningLevel = 0;
	uint8_t reasoningOption1 = 0;
	uint8_t reasoningOption2 = 0;
	uint8_t reasoningOption3 = 0;

	uint32_t __cdecl Effect_Reasoning_Proxy(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		uint8_t state = GameData::GetEffectState();

		switch (state)
		{
		case 0x80:
		{
			if (GameData::IsAI(selfParam.playerIdx))
			{
				reasoningLevel = FUN::GetRandomNumber(12) + 1;
				uint8_t firstRoll = reasoningLevel > 1 ? reasoningLevel / 2 : 1;
				FUN::W_RollRiggedDice(selfParam.oppIdx, selfParam.oppIdx, firstRoll);
				if (reasoningLevel > 1) FUN::W_RollRiggedDice(selfParam.oppIdx, selfParam.oppIdx, reasoningLevel - firstRoll);

				return 0x7b;
			}
			else
			{
				FUN::W_ShowDialog("Declare a level between 1 and 12:", DialogMode::OK);

				return 0x7f;
			}
		};
		case 0x7f:
		{
			FUN::W_ShowDialog(
				"Choose a number\n"
				"\n"
				"  1-6\n"
				"  7-12\n", DialogMode::OPTION2);

			return 0x7e;
		}
		case 0x7e:
		{
			reasoningOption1 = GameData::GetDialogResult();
			if (reasoningOption1 == 0)
			{
				FUN::W_ShowDialog(
					"Choose a number\n"
					"\n"
					"  1-3\n"
					"  4-6\n", DialogMode::OPTION2);
			}
			else
			{
				FUN::W_ShowDialog(
					"Choose a number\n"
					"\n"
					"  7-9\n"
					"  10-12\n", DialogMode::OPTION2);
			}

			return 0x7d;
		}
		case 0x7d:
		{
			reasoningOption2 = GameData::GetDialogResult();
			if (reasoningOption1 == 0)
			{
				if (reasoningOption2 == 0)
				{
					FUN::W_ShowDialog(
						"Choose a number\n"
						"\n"
						"  1\n"
						"  2\n"
						"  3\n", DialogMode::OPTION3);
				}
				else
				{
					FUN::W_ShowDialog(
						"Choose a number\n"
						"\n"
						"  4\n"
						"  5\n"
						"  6\n", DialogMode::OPTION3);
				}
			}
			else
			{
				if (reasoningOption2 == 0)
				{
					FUN::W_ShowDialog(
						"Choose a number\n"
						"\n"
						"  7\n"
						"  8\n"
						"  9\n", DialogMode::OPTION3);
				}
				else
				{
					FUN::W_ShowDialog(
						"Choose a number\n"
						"\n"
						"  10\n"
						"  11\n"
						"  12\n", DialogMode::OPTION3);
				}
			}

			return 0x7c;
		}
		case 0x7c:
		{
			reasoningOption3 = GameData::GetDialogResult();
			reasoningLevel = reasoningOption1 * 6 + reasoningOption2 * 3 + reasoningOption3 + 1;

			return 0x7b;
		}
		case 0x7b:
		{
			if (duel->players[selfParam.oppIdx].cardsInDeck == 0) return 0;
			uint16_t cardIntID = duel->players[selfParam.oppIdx].deck[0].GetIntID();

			FUN::W_RevealTopCard(selfParam.oppIdx, FUN::GetCardID(cardIntID), 0);
			if (FUN::GetMonsterType(cardIntID) > 0x14 || FUN::HasInherentSummon(cardIntID) != 0 || FUN::CanBeSummonedByEffect(selfParam.oppIdx, cardIntID) == 0)
			{
				FUN::MillCards(selfParam.oppIdx, 1, 0);

				return 0x7b;
			}
			return 0x7a;
		}
		case 0x7a:
		{
			auto& card = duel->players[selfParam.oppIdx].deck[0];

			if (card.GetLevel() == reasoningLevel) FUN::MillCards(selfParam.oppIdx, 1, 0);
			else FUN::SpecialSummon(selfParam.oppIdx, (uint32_t*)&card.fullValue, 1, 0x20, 0x0D, 0);

			return 0;
		}
		}

	}
	uint32_t __cdecl Condition_Reasoning(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (duel->players[selfParam.playerIdx].cardsInDeck == 0) return 0;
		if (FUN::CanPlayerSummon(selfParam.playerIdx) == 0) return 0;
		if (FUN::NumOfEmptyValidSummonZones(selfParam.playerIdx) == 0) return 0;

		return 1;
	}
	void __stdcall OnEffectActivated(unsigned int* srcParam, uint8_t respondingSide)
	{
		FUN::Param src(srcParam);
		if (FUN::GetCardID(src.cardIntID) == REASONING)
		{
			if (src.playerIdx == respondingSide) return;

			uint16_t cardIntID = FUN::GetCardIntID(REASONING);
			FUN::FlashCardPortrait(respondingSide ^ 1, cardIntID, src.zoneIdx);

			uint32_t pack = ((uint32_t)(0 & 0x1F) | ((uint32_t)respondingSide << 0xf) | 0x0A20u) << 16 | FUN::GetCardIntID(REASONING_PROXY);
			FUN::ChainEffect(pack, 0, srcParam, 1);
		}
	}
}

void Install_Reasoning()
{
	Register_OnEffectActivated(OnEffectActivated);

	Register_EffectScript({
		.CardID = REASONING,
		.Effect = 0,
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(&Condition_Reasoning),
		.Cost = 0,
		.Target = 0
		});
	Register_EffectScript({
		.CardID = REASONING_PROXY,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_Reasoning_Proxy),
		.AppliesTo = 0,
		.Condition = 0,
		.Cost = 0,
		.Target = 0
		});

}