#include "RandomCards.h"

namespace
{
	constexpr uint16_t PLAGUESPREADER_ZOMBIE = Cards::ALINSECTION;

	uint8_t pzZone = 0;

	uint32_t __cdecl Effect_PZ(unsigned int* param, int param2, int param3);
	uint32_t __cdecl Condition_PZ(unsigned int* param, int param2, int param3);
	uint32_t __cdecl Cost_PZ(unsigned int* param, int param2, int param3);

	bool __stdcall PZ_FieldLeave(uint32_t side, uint32_t zone, uint32_t* dest, uint32_t* action, uint32_t* effectIntID);
	void __stdcall PZ_BanishOnDeath();

	uint32_t __cdecl Effect_PZ(unsigned int* param, int param2, int param3)
	{
		FUN::Param funParam(param);

		if (FUN::CanPlayerSummon(funParam.playerIdx) == 0) return 0;
		if (FUN::NumOfEmptyValidSummonZones(funParam.playerIdx) == 0) return 0;
		if (funParam.location != Location::GRAVE) return 0;

		uint8_t state = GameData::GetEffectState();
		switch (state)
		{
		case 0x80:
		{
			for (size_t i = 0; i < duel->players[funParam.playerIdx].cardsInGrave; i++)
			{
				auto& card = duel->players[funParam.playerIdx].grave[i];

				if (card.GetInstance() == funParam.instance)
				{
					pzZone = FUN::GetSummonZone(funParam.playerIdx);

					FUN::SpecialSummon(funParam.playerIdx, (uint32_t*)&card.fullValue, 1, 0x20, 0x0E, 0);
				}
			}
			return 0x7f;
		}
		case 0x7f:
		{
			FUN::W_AddEffectEntityToZone(funParam.playerIdx, pzZone, FUN::GetCardIntID(PLAGUESPREADER_ZOMBIE), 0xB | (0 << 8));

			return 0;
		}
		default:
		{
			return 0;
		}

		}
	}
	uint32_t __cdecl Condition_PZ(unsigned int* param, int param2, int param3)
	{
		FUN::Param funParam(param);

		if (FUN::CanPlayerSummon(funParam.playerIdx) == 0) return 0;
		if (FUN::NumOfEmptyValidSummonZones(funParam.playerIdx) == 0) return 0;
		if (duel->players[funParam.playerIdx].cardsInHand == 0) return 0;

		return 1;
	}
	uint32_t __cdecl Cost_PZ(unsigned int* param, int param2, int param3)
	{
		FUN::Param funParam(param);

		uint8_t sub = GameData::GetEffectSubState();

		switch (sub)
		{
		case 0:
		{
			FUN::ShowDialog("Select @31@0 card from your hand to place on top of your Deck.");
			GameData::SetEffectSubState(1);
		}break;
		case 1:
		{
			if (GameData::GetSelectedLocation() != Location::HAND) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

			FUN::W_PutCardFromLocationToDeck(funParam.playerIdx, Location::HAND, GameData::GetSelectedColumn(), true);
			GameData::SetEffectSubState(0);
			return 1;
		}break;
		}

		return 0;
	}
	bool __stdcall PZ_FieldLeave(uint32_t side, uint32_t zone, uint32_t* dest, uint32_t* action, uint32_t* effectIntID)
	{
		auto& card = duel->players[side].cardZones[zone].card;
		if (card.GetIntID() != 0)
		{
			if (card.GetCardID() == PLAGUESPREADER_ZOMBIE)
			{
				if (FUN::HasEffectEntiry(side, zone, PLAGUESPREADER_ZOMBIE) != 0)
				{
					*action |= 0x10000; // Banish flag
				}
			}
		}
		return false;
	}
	void __stdcall PZ_BanishOnDeath()
	{
		for (size_t side = 0; side < 2; side++)
		{
			if ((battleResult->sides[side].ResultFlags & 0x10) != 0)
			{
				uint8_t zone = (side == (battleResult->StateFlags & 1)) ? battleResult->GetZone(0) : battleResult->GetZone(1);

				if (duel->players[side].cardZones[zone].card.GetCardID() == PLAGUESPREADER_ZOMBIE && FUN::HasEffectEntiry(side, zone, PLAGUESPREADER_ZOMBIE) != 0)
				{
					FUN::FieldMaskGenerator maskGen;
					maskGen.zones[side][zone] = true;
					uint8_t block[32] = {};

					FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xf, 0);
				}
			}
		}
	}
}

void Install_PlagueSpreaderZombie()
{
	Register_ActivatableGraveEffect(PLAGUESPREADER_ZOMBIE);
	Register_OnCardLeavingField(PZ_FieldLeave);
	Register_AfterDamageCalculation(PZ_BanishOnDeath);

	Register_EffectScript({
		.CardID = PLAGUESPREADER_ZOMBIE,
		.Effect = reinterpret_cast<uintptr_t>(Effect_PZ),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(Condition_PZ),
		.Cost = reinterpret_cast<uintptr_t>(Cost_PZ),
		.Target = 0
		});
}