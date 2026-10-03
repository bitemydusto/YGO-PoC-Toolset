#include "GOAT.h"

namespace
{
	constexpr uint16_t LAVA_GOLEM = Cards::DARK_TITAN_OF_TERROR;

	int innerState = 0;
	uint8_t firstZone = 0;

	uint32_t __cdecl Effect_LG(unsigned int* self, unsigned int* source, int mode);

	uint32_t __stdcall SummonStates();
	bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col);
	bool SummonCondition(uint32_t playerIdx);
	void __stdcall StandbyPhase();

	uint32_t __cdecl Effect_LG(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param funParam(self);

		FUN::DealEffectDamage(funParam.playerIdx, 1000);

		return 0;
	}
	bool SummonCondition(uint32_t playerIdx)
	{
		if ((Utils::ReadUint8((void*)0x00A57176) & 2) != 0) return false; // Already normal summoned this turn
		if (FUN::CanPlayerSummon(playerIdx) == 0) return false;
		if (FUN::IsCardOnField(Cards::MASK_OF_RESTRICT) != 0) return false;
		int n = 0;
		for (size_t i = 0; i < 5; i++)
		{
			if (duel->players[0].cardZones[i].card.GetIntID() != 0) n++;
		}
		if (n < 2) return false;
		return true;
	}
	bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col)
	{
		if (side == playerIdx) return false;
		if (col > 4) return false;
		if (duel->players[playerIdx ^ 1].cardZones[col].card.GetIntID() == 0) return false;

		return true;
	}
	uint32_t __stdcall SummonStates()
	{
		switch (innerState)
		{
			case 0:
			{
				FUN::ShowDialog("You must Tribute @32@0 monsters from your opponent's side of the field. Do you wish to @2Summon@0?");
				FUN::ShowDialogOptions(1, 0);
				innerState = 1;
			}break;
			case 1:
			{
				if (GameData::GetDialogResult() == 0)
				{
					innerState = 0;
					return 1;
				}
				innerState = 2;
			}break;
			case 2:
			{
				if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

				uint8_t side = GameData::GetSelectedSide();
				uint8_t col = GameData::GetSelectedColumn();

				if (!CanBeTributed(GameData::GetTurnPlayer(), side, col)) return 0;

				if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

				FUN::MarkZoneAsTributed(side, col);

				firstZone = col;

				innerState = 3;
			}break;
			case 3:
			{
				if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

				uint8_t side = GameData::GetSelectedSide();
				uint8_t col = GameData::GetSelectedColumn();

				if (col == firstZone) return 0;
				if (!CanBeTributed(GameData::GetTurnPlayer(), side, col)) return 0;

				if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

				FUN::MarkZoneAsTributed(side, col);

				FUN::TributeSelected(side, firstZone);
				FUN::TributeSelected(side, col);

				innerState = 4;
			}break;
			case 4:
			{
				uint16_t sel = GameData::GetSelectedSoFar();
				GameData::SetSelectedSoFar(sel & 0xff00);

				uint32_t summonParam = GameData::GetSummonParam();
				GameData::SetSummonParam(summonParam & 0xf1ffffff);

				uint16_t choice = (Utils::ReadUint8((void*)0x00a57804) >> 3) & 1;

				uint32_t param2 = Utils::ReadUint8((void*)0x00a5780c);
				uint32_t param3 = FUN::GetSummonZone(GameData::GetTurnPlayer() ^ 1);
				uint32_t param5 = (choice == 0) ? 1 : 0;



				FUN::W_SS_HandToOpp(GameData::GetTurnPlayer(), param2, param3, 0, param5);
				uint8_t x = Utils::ReadUint8((void*)0x00A57176);
				Utils::WriteUint8((void*)0x00A57176, x | 2); // Set already normal summoned this turn flag

				innerState = 0;
				return 1;
			}
		}

		return 0;
	}
	void __stdcall StandbyPhase()
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 5; j++)
			{
				uint16_t cardIntID = duel->players[i].cardZones[j].card.GetIntID();
				uint16_t cardID = FUN::GetCardID(cardIntID);
				if (cardID == LAVA_GOLEM)
				{
					if (UsedEffectThisTurn(i, j)) continue;
					if (GameData::GetTurnPlayer() != i) continue;
					SetOncePerTurnFlag(i, j);

					uint32_t pack = ((uint32_t)(j & 0x1F) | ((uint32_t)i << 0xf) | 0x0A20u) << 16 | cardIntID;
					uint32_t inst = duel->players[i].cardZones[j].card.GetInstance();

					FUN::QueueEffect(pack, inst, 0);
				}
			}
		}
	}

}

void Install_LavaGolem()
{
	Register_InherentSpecialSummon(LAVA_GOLEM, true);
	Register_SpecialSummonCondition(LAVA_GOLEM, SummonCondition);
	Register_InitialSummonState(LAVA_GOLEM, 0x38, false);
	Register_SummonState(0x38, SummonStates);
	Register_Phase(1, StandbyPhase);

	Register_EffectScript({
		.CardID = LAVA_GOLEM,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_LG),
		.AppliesTo = 0,
		.Condition = 0,
		.Cost = 0,
		.Target = 0
		});
}
