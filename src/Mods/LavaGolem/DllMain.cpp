#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

const uint16_t LAVA_GOLEM = Cards::DARK_TITAN_OF_TERROR;
int innerState = 0;
uint8_t firstZone = 0;

uint32_t __cdecl Effect_LG(unsigned int* param, int param2, int param3);
void Start();

uint32_t __stdcall SummonStates();
bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col);
bool SummonCondition(uint32_t playerIdx);
void __stdcall StandbyPhase();
void __stdcall EndPhase();



DWORD WINAPI MainThread(LPVOID lpParam)
{
    Sleep(500);
    Start();

    return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
        CreateThread(0, 0, &MainThread, 0, 0, NULL);
    }

    return TRUE;
}
void Start()
{
	Register_InherentSpecialSummon(LAVA_GOLEM);
	Register_SpecialSummonCondition(LAVA_GOLEM, SummonCondition);
	Register_InitialSummonState(LAVA_GOLEM, 0x38);
	Register_SummonState(0x38, SummonStates);
	Register_Phase(1, StandbyPhase);
	Register_Phase(6, EndPhase);


	Utils::EffectScript script;
	script.CardID = LAVA_GOLEM;
	script.Effect = reinterpret_cast<uintptr_t>(&Effect_LG);
	script.AppliesTo = 0;
	script.Condition =0;
	script.Cost = 0;
	script.Target = 0;
	ReplaceEffectScript(Cards::RAIMEI, script);
}
uint32_t __cdecl Effect_LG(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	FUN::DealEffectDamage(funParam.playerIdx, 1000);

	return 0;
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

			if (!CanBeTributed(0, side, col)) return 0;

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
			if (!CanBeTributed(0, side, col)) return 0;

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
			uint32_t param3 = FUN::GetSummonZone(0);
			uint32_t param5 = (choice == 0) ? 1 : 0;



			FUN::W_SS_HandToOpp(1, param2, param3, 0, param5);
			uint8_t x = Utils::ReadUint8((void*)0x00A57176);
			Utils::WriteUint8((void*)0x00A57176, x | 2); // Set already normal summoned this turn flag

			innerState = 0;
			return 1;
		}
	}

	return 0;
}
bool SummonCondition(uint32_t playerIdx)
{
	GameData::Duel duel = GameData::GetDuel();

	if ((Utils::ReadUint8((void*)0x00A57176) & 2) != 0) return false; // Already normal summoned this turn
	if (FUN::CanPlayerSummon(playerIdx) == 0) return false;
	if (FUN::IsCardOnField(Cards::MASK_OF_RESTRICT) != 0) return false;
	int n = 0;
	for (size_t i = 0; i < 5; i++)
	{
		if (duel.players[0].monsterZones[i].card.intID != 0) n++;
	}
	if (n < 2) return false;
	return true;
}
bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col)
{
	GameData::Duel duel = GameData::GetDuel();

	if (side != 0) return false;
	if (col > 4) return false;
	if (duel.players[0].monsterZones[col].card.intID == 0) return false;

	return true;
}
void __stdcall StandbyPhase()
{
	GameData::Duel duel = GameData::GetDuel();

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			uint16_t cardIntID = duel.players[i].monsterZones[j].card.intID;
			uint16_t cardID = FUN::GetCardID(cardIntID);
			if (cardID == LAVA_GOLEM)
			{
				if ((duel.players[i].monsterZones[j].effectIDs[31] & 0x1) == 0x1) return; // Already used its effect this turn
				Utils::WriteUint16((void*)(0x00a55d64 + i * 0xD44 + 0x10 + 0x90 * j + 0x4A), 0x1); // Set custom once per turn flag



				uint32_t dword = duel.players[i].monsterZones[j].card.fullValue;
				uint32_t owner = (dword >> 12) & 1;
				uint32_t instHi = (dword >> 24) & 0x7F;
				uint16_t flag = instHi * 2 + owner;

				FUN::InvokeEffect((j & 0x1F | i << 0xF | 0x0A20) << 0x10 | (cardIntID & 0xFFF), flag & 0xFF, flag);
			}
		}
	}
}
void __stdcall EndPhase()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			Utils::WriteUint16((void*)(0x00a55d64 + i * 0xD44 + 0x10 + j * 0x90 + 0x4A), 0x0);
		}
	}
}

