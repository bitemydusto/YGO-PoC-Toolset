#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

const uint16_t XYZ_DRAGON_CANNON = Cards::KANIKABUTO;
const uint16_t X_HEAD_CANNON = Cards::GANIGUMO;
const uint16_t Y_DRAGON_HEAD = Cards::KAMAKIRIMAN;
const uint16_t Z_METAL_TANK = Cards::MASKED_CLOWN;

GameData::Duel* duel = GameData::GetDuel();
int innerState = 0;

struct Material
{
	uint8_t zone;
	uint16_t cardID;
};
Material materials[3];

void Start();

bool __cdecl AppliesTo_XYZ(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_XYZ(unsigned int* param, int param2, int param3);
uint32_t __cdecl Cost_XYZ(unsigned int* param, int param2, int param3);

bool CanBeSummoned(uint32_t playerIdx);
uint32_t __stdcall SummonStates();

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
    Register_ExtraSummonMonster(XYZ_DRAGON_CANNON, CanBeSummoned, SummonStates);
	Register_ActivatableEffect(XYZ_DRAGON_CANNON);
	Register_UnRevivable(XYZ_DRAGON_CANNON);
	Register_CanBeSummonedByEffect(XYZ_DRAGON_CANNON, false);

	EffectScript scriptXYZ;
	scriptXYZ.CardID = XYZ_DRAGON_CANNON;
	scriptXYZ.Effect = 0x00585C10;
	scriptXYZ.AppliesTo = reinterpret_cast<uintptr_t>(&AppliesTo_XYZ);
	scriptXYZ.Condition = reinterpret_cast<uintptr_t>(&Condition_XYZ);
	scriptXYZ.Cost = reinterpret_cast<uintptr_t>(&Cost_XYZ);
	scriptXYZ.Target = 0x00596570;
	Register_EffectScript(scriptXYZ);

}
bool __cdecl AppliesTo_XYZ(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	uint8_t zoneIdx = param2 >> 8 & 0xff;
	uint8_t sideIdx = (param2 & 0xff) & 1;

	if (10 < zoneIdx) return false;
	if (sideIdx == funParam.playerIdx) return false;
	if (!FUN::CanCardBeTargeted(sideIdx, zoneIdx)) return false;
	if (duel->players[sideIdx].monsterZones[zoneIdx].card.GetIntID() == 0) return false;

	return true;
}
uint32_t __cdecl Condition_XYZ(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	if (duel->players[funParam.playerIdx].cardsInHand == 0) return 0;
	uint8_t side = funParam.playerIdx ^ 1;

	int n = 0;
	for (size_t i = 0; i < 11; i++)
	{
		uint16_t cardIntID = duel->players[side].monsterZones[i].card.GetIntID();
		if (cardIntID != 0) n++;
	}

	if (n > 0) return 1;
	return 0;
}
uint32_t __cdecl Cost_XYZ(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	FUN::SelectCardsToDiscard(funParam.playerIdx, 1, 0, 0);

	return 1;
}
bool CanBeSummoned(uint32_t playerIdx)
{
	bool hasX = false;
	bool hasY = false;
	bool hasZ = false;

	for (size_t i = 0; i < 5; i++)
	{
		uint16_t cardIntID = duel->players[playerIdx].monsterZones[i].card.GetIntID();
		if (cardIntID != 0)
		{
			uint16_t cardID = FUN::GetCardID(cardIntID);
			if (cardID == X_HEAD_CANNON) hasX = true;
			if (cardID == Y_DRAGON_HEAD) hasY = true;
			if (cardID == Z_METAL_TANK) hasZ = true;
		}
	}
	return hasX && hasY && hasZ;
}
uint32_t __stdcall SummonStates()
{
	switch (innerState)
	{
		case 0:
		{
			FUN::ShowDialog("Banish the listed materials from your side of the field.");
			innerState = 1;
		}break;
		case 1:
		{
			if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

			uint8_t side = GameData::GetSelectedSide();
			uint8_t col = GameData::GetSelectedColumn();

			uint16_t intID = duel->players[side].monsterZones[col].card.GetIntID();
			if (intID == 0) return 0;

			uint16_t cardID = FUN::GetCardID(intID);
			if (!(cardID == X_HEAD_CANNON || cardID == Y_DRAGON_HEAD || cardID == Z_METAL_TANK)) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

			materials[0].zone = col;
			materials[0].cardID = cardID;

			FUN::FieldMaskGenerator maskGen;
			maskGen.zones[side][col] = true;

			uint8_t block[32] = {};
			FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xF, 0);

			innerState = 2;
		}break;
		case 2:
		{
			if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

			uint8_t side = GameData::GetSelectedSide();
			uint8_t col = GameData::GetSelectedColumn();

			uint16_t intID = duel->players[side].monsterZones[col].card.GetIntID();
			if (intID == 0) return 0;

			uint16_t cardID = FUN::GetCardID(intID);
			if (!(cardID == X_HEAD_CANNON || cardID == Y_DRAGON_HEAD || cardID == Z_METAL_TANK)) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;
			if (materials[0].zone == col || materials[0].cardID == cardID) return 0;

			materials[1].zone = col;
			materials[1].cardID = cardID;

			FUN::FieldMaskGenerator maskGen;
			maskGen.zones[side][col] = true;

			uint8_t block[32] = {};
			FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xF, 0);

			innerState = 3;
		}break;
		case 3:
		{
			if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

			uint8_t side = GameData::GetSelectedSide();
			uint8_t col = GameData::GetSelectedColumn();

			uint16_t intID = duel->players[side].monsterZones[col].card.GetIntID();
			if (intID == 0) return 0;

			uint16_t cardID = FUN::GetCardID(intID);
			if (!(cardID == X_HEAD_CANNON || cardID == Y_DRAGON_HEAD || cardID == Z_METAL_TANK)) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;
			if (materials[0].zone == col || materials[1].zone == col) return 0;
			if (materials[0].cardID == cardID || materials[1].cardID == cardID) return 0;

			materials[2].zone = col;
			materials[2].cardID = cardID;

			FUN::FieldMaskGenerator maskGen;
			maskGen.zones[side][col] = true;

			uint8_t block[32] = {};
			FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xF, 0);

			innerState = 4;
		}break;
		case 4:
		{
			uint32_t* cardDword = nullptr;
			for (size_t i = 0; i < duel->players[GameData::GetTurnPlayer()].cardsInExtra; i++)
			{
				uint16_t cardID = FUN::GetCardID(duel->players[GameData::GetTurnPlayer()].extra[i].GetIntID());
				if (cardID == XYZ_DRAGON_CANNON)
				{
					cardDword = &(duel->players[GameData::GetTurnPlayer()].extra[i].fullValue);
					break;
				}
			}

			FUN::SpecialSummon(1, cardDword, 1, 0x20, 0x0C, 0);

			innerState = 0;
			return 1;
		}
	}

	return 0;
}