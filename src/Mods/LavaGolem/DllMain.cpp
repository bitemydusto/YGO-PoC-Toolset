#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

GameData::Duel* duel = GameData::GetDuel();

const uint16_t LAVA_GOLEM = Cards::DARK_TITAN_OF_TERROR;
const uint16_t MIRAGE_OF_NIGHTMARE = Cards::THE_DRDEK;
const uint16_t PLAGUESPREADER_ZOMBIE = Cards::ALINSECTION;

int innerState = 0;
uint8_t firstZone = 0;
uint8_t pzZone = 0;

uint32_t __cdecl Effect_LG(unsigned int* param, int param2, int param3);

uint32_t __cdecl Effect_Mirage(unsigned int* param, int param2, int param3);

uint32_t __cdecl Effect_PZ(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_PZ(unsigned int* param, int param2, int param3);
uint32_t __cdecl Cost_PZ(unsigned int* param, int param2, int param3);


void Start();

uint32_t __stdcall SummonStates();
bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col);
bool SummonCondition(uint32_t playerIdx);
void __stdcall StandbyPhase();
void __stdcall StandbyPhaseMirage();
void __stdcall EndPhase();
bool __stdcall PZ_FieldLeave(uint32_t side, uint32_t zone, uint32_t* dest, uint32_t* action, uint32_t* effectIntID);




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
	Register_InherentSpecialSummon(LAVA_GOLEM, true);
	Register_SpecialSummonCondition(LAVA_GOLEM, SummonCondition);
	Register_InitialSummonState(LAVA_GOLEM, 0x38, false);
	Register_SummonState(0x38, SummonStates);
	Register_Phase(1, StandbyPhase);
	Register_Phase(5, EndPhase);

	Register_Phase(1, StandbyPhaseMirage);

	Register_ActivatableGraveEffect(PLAGUESPREADER_ZOMBIE);
	Register_OnCardLeavingField(PZ_FieldLeave);


	Utils::EffectScript script;
	script.CardID = LAVA_GOLEM;
	script.Effect = reinterpret_cast<uintptr_t>(&Effect_LG);
	script.AppliesTo = 0;
	script.Condition =0;
	script.Cost = 0;
	script.Target = 0;
	Register_EffectScript(script);

	Utils::EffectScript scriptMirage;
	scriptMirage.CardID = MIRAGE_OF_NIGHTMARE;
	scriptMirage.Effect = reinterpret_cast<uintptr_t>(&Effect_Mirage);
	scriptMirage.AppliesTo = 0;
	scriptMirage.Condition = 0;
	scriptMirage.Cost = 0;
	scriptMirage.Target = 0;
	Register_EffectScript(scriptMirage);

	Utils::EffectScript scriptPZ;
	scriptPZ.CardID = PLAGUESPREADER_ZOMBIE;
	scriptPZ.Effect = reinterpret_cast<uintptr_t>(&Effect_PZ);
	scriptPZ.AppliesTo = 0;
	scriptPZ.Condition = reinterpret_cast<uintptr_t>(&Condition_PZ);
	scriptPZ.Cost = reinterpret_cast<uintptr_t>(&Cost_PZ);
	scriptPZ.Target = 0;
	Register_EffectScript(scriptPZ);
}
uint32_t __cdecl Effect_LG(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	FUN::DealEffectDamage(funParam.playerIdx, 1000);

	return 0;
}
uint32_t __cdecl Effect_Mirage(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	if (GameData::GetPhase() > 3 || funParam.zoneIdx > 9) return 0;

	uint16_t cardIntID = duel->players[funParam.playerIdx].monsterZones[funParam.zoneIdx].card.GetIntID();
	uint16_t cardID = FUN::GetCardID(cardIntID);

	if (cardID != MIRAGE_OF_NIGHTMARE) return 0;

	if (GameData::GetTurnPlayer() == funParam.playerIdx)
	{
		for (size_t i = 0; i < duel->players[funParam.playerIdx].monsterZones[funParam.zoneIdx].effectCount; i++)
		{
			uint16_t effectIntID = duel->players[funParam.playerIdx].monsterZones[funParam.zoneIdx].effectIDs[i];
			uint16_t effectID = FUN::GetCardID(effectIntID);
			if (effectID == MIRAGE_OF_NIGHTMARE)
			{
				int n = duel->players[funParam.playerIdx].monsterZones[funParam.zoneIdx].effectEntries[i].value;

				if (n != 0)
				{
					if (duel->players[funParam.playerIdx].cardsInHand < n)
					{
						n = duel->players[funParam.playerIdx].cardsInHand;
					}
					FUN::DiscardRandomCard(funParam.playerIdx, 0, n);
					FUN::RemoveEffectEntity(funParam.playerIdx, funParam.location, i);

					return 0;

				}

			}
		}
	}
	else
	{
		int n = 4 - duel->players[funParam.playerIdx].cardsInHand;

		FUN::W_AddEffectEntityToZone(funParam.playerIdx, funParam.zoneIdx, funParam.cardIntID, (n << 8) | 0xB);
		FUN::DrawCards(funParam.playerIdx, n);
	}


	return 0;
}
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
bool SummonCondition(uint32_t playerIdx)
{
	if ((Utils::ReadUint8((void*)0x00A57176) & 2) != 0) return false; // Already normal summoned this turn
	if (FUN::CanPlayerSummon(playerIdx) == 0) return false;
	if (FUN::IsCardOnField(Cards::MASK_OF_RESTRICT) != 0) return false;
	int n = 0;
	for (size_t i = 0; i < 5; i++)
	{
		if (duel->players[0].monsterZones[i].card.GetIntID() != 0) n++;
	}
	if (n < 2) return false;
	return true;
}
bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col)
{
	if (side == playerIdx) return false;
	if (col > 4) return false;
	if (duel->players[playerIdx ^ 1].monsterZones[col].card.GetIntID() == 0) return false;

	return true;
}
void __stdcall StandbyPhase()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			uint16_t cardIntID = duel->players[i].monsterZones[j].card.GetIntID();
			uint16_t cardID = FUN::GetCardID(cardIntID);
			if (cardID == LAVA_GOLEM)
			{
				if ((duel->players[i].monsterZones[j].effectIDs[31] & 0x1) == 0x1) return; // Already used its effect this turn
				if (GameData::GetTurnPlayer() != i) return; // Not the turn of the player who controls it
				Utils::WriteUint16((void*)(0x00a55d64 + i * 0xD44 + 0x10 + 0x90 * j + 0x4A), 0x1); // Set custom once per turn flag



				uint32_t pack = ((uint32_t)(j & 0x1F) | ((uint32_t)i << 0xf) | 0x0A20u) << 16 | cardIntID;

				FUN::InvokeEffect(pack, 0, 0);
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
void __stdcall StandbyPhaseMirage()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 5; j < 10; j++)
		{
			uint16_t cardIntID = duel->players[i].monsterZones[j].card.GetIntID();
			uint16_t cardID = FUN::GetCardID(cardIntID);
			if (cardID == MIRAGE_OF_NIGHTMARE)
			{
				if (GameData::GetTurnPlayer() != i && duel->players[i].cardsInHand > 4) return;

				uint32_t pack = ((uint32_t)(j & 0x1F) | ((uint32_t)i << 0xf) | 0x0A20u) << 16 | cardIntID;

				FUN::InvokeEffect(pack, 0, 0);
			}

		}
	}
}
bool __stdcall PZ_FieldLeave(uint32_t side, uint32_t zone, uint32_t* dest, uint32_t* action, uint32_t* effectIntID)
{
	auto& card = duel->players[side].monsterZones[zone].card;
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
