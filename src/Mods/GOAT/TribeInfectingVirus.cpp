#include "GOAT.h"

namespace
{
	constexpr uint16_t TRIBE = 0x800;

	uint8_t declaredType = 0;

	uint32_t __cdecl Effect_Tribe(unsigned int* self, unsigned int* source, int mode);
	uint32_t __cdecl Condition_Tribe(unsigned int* self, unsigned int* source, int mode);
	uint32_t __cdecl Cost_Tribe(unsigned int* self, unsigned int* source, int mode);

	uint32_t __cdecl Effect_Tribe(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		FUN::FieldMaskGenerator maskGen;
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				uint16_t cardIntID = duel->players[i].cardZones[j].card.GetIntID();
				if (cardIntID != 0)
				{
					if (FUN::GetMonsterType(cardIntID) == declaredType)
					{
						maskGen.zones[i][j] = true;
					}
				}
			}
		}
		FUN::SendCardFromField(selfParam.block, maskGen.GenerateMask(), 0x0E, 2);
		FUN::W_RevealTopCard(selfParam.playerIdx, 0x777, 0);

		return 0;
	}
	uint32_t __cdecl Condition_Tribe(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		return duel->players[selfParam.playerIdx].cardsInHand > 0;
	}
	uint32_t __cdecl Cost_Tribe(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		auto sub = GameData::GetEffectSubState();

		switch (sub)
		{
		case 0:
		{
			FUN::SelectCardsToDiscard(selfParam.playerIdx, 1, 0, 0);

			GameData::SetEffectSubState(1);
		}break;
		case 1:
		{
			FUN::W_ShowDialog("Select a @3Monster Type:@0", DialogMode::TYPE);

			GameData::SetEffectSubState(2);
		}break;
		case 2:
		{
			if (FUN::SelectionConfirmed() != 0) return 0;

			declaredType = GameData::GetDialogResult() + 1;

			GameData::SetEffectSubState(0);
			return 1;
		}
		}

		return 0;
	}
}

void Install_Tribe()
{
	Register_ActivatableEffect(TRIBE);

	Register_EffectScript({
		.CardID = TRIBE,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_Tribe),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(&Condition_Tribe),
		.Cost = reinterpret_cast<uintptr_t>(&Cost_Tribe),
		.Target = 0
		});
}