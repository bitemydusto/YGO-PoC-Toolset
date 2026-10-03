#include "GOAT.h"

namespace
{
	constexpr uint16_t REASONING = 0x801;

	uint8_t reasoningLevel = 0;
	uint8_t reasoningOption1 = 0;
	uint8_t reasoningOption2 = 0;
	uint8_t reasoningOption3 = 0;

	uint32_t __cdecl Effect_Reasoning(unsigned int* self, unsigned int* source, int mode);
	uint32_t __cdecl Condition_Reasoning(unsigned int* self, unsigned int* source, int mode);

	uint32_t __cdecl Effect_Reasoning(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		uint8_t state = GameData::GetEffectState();

		switch (state)
		{
		case 0x80:
		{
			if (GameData::IsAI(selfParam.oppIdx))
			{
				reasoningLevel = FUN::GetRandomNumber(12) + 1;
				uint8_t firstRoll = reasoningLevel > 1 ? reasoningLevel / 2 : 1;
				FUN::W_RollRiggedDice(selfParam.oppIdx, selfParam.oppIdx, firstRoll);
				if (reasoningLevel > 1) FUN::W_RollRiggedDice(selfParam.oppIdx, selfParam.oppIdx, reasoningLevel - firstRoll);

				return 0x7b;
			}
			else
			{
				*(uint8_t*)(0x00a577fa) ^= 1;
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

			*(uint8_t*)(0x00a577fa) ^= 1;
			return 0x7b;
		}
		case 0x7b:
		{
			uint16_t cardIntID = duel->players[selfParam.playerIdx].deck[0].GetIntID();

			FUN::W_RevealTopCard(selfParam.playerIdx, FUN::GetCardID(cardIntID), 0);
			if (FUN::GetMonsterType(cardIntID) > 0x14 || FUN::HasInherentSummon(cardIntID) != 0 || FUN::CanBeSummonedByEffect(selfParam.playerIdx, cardIntID) == 0)
			{
				FUN::MillCards(selfParam.playerIdx, 1, 0);

				return 0x7b;
			}
			return 0x7a;
		}
		case 0x7a:
		{
			auto& card = duel->players[selfParam.playerIdx].deck[0];

			if (card.GetLevel() == reasoningLevel) FUN::MillCards(selfParam.playerIdx, 1, 0);
			else FUN::SpecialSummon(selfParam.playerIdx, (uint32_t*)&card.fullValue, 1, 0x20, 0x0D, 0);

			return 0;
		}
		}

	}
	uint32_t __cdecl Condition_Reasoning(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (duel->players[selfParam.playerIdx].cardsInDeck == 0) return 0;
		if (FUN::CanPlayerSummon(selfParam.playerIdx) == 0) return 0;

		return 1;
	}
}

void Install_Reasoning()
{
	Register_EffectScript({
		.CardID = REASONING,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_Reasoning),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(&Condition_Reasoning),
		.Cost = 0,
		.Target = 0
		});
}