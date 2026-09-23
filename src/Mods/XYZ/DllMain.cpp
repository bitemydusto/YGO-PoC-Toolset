#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

const uint16_t XYZ_DRAGON_CANNON = Cards::KANIKABUTO;
const uint16_t X_HEAD_CANNON = Cards::GANIGUMO;
const uint16_t Y_DRAGON_HEAD = Cards::KAMAKIRIMAN;
const uint16_t Z_METAL_TANK = Cards::MASKED_CLOWN;

auto duel = GameData::GetDuel();
auto battleResult = GameData::GetBattleResult();
int innerState = 0;
uint8_t destZoneYZ = 0;

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

uint32_t __cdecl Effect_YZ(unsigned int* param, int param2, int param3);
bool __cdecl AppliesTo_YZ(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_YZ(unsigned int* param, int param2, int param3);
uint32_t __cdecl Target_YZ(unsigned int* param, int param2, int param3);

bool CanBeSummoned(uint32_t playerIdx);
uint32_t __stdcall SummonStates();
void __stdcall StatChange_Y(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
void __stdcall StatChange_Z(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
void __stdcall Protection();
bool __stdcall EffectProtection(uint32_t side, uint32_t zone, uint32_t dest, uint32_t flags, uint32_t effectIntID);

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

	Register_ActivatableEffect(Y_DRAGON_HEAD);
	Register_ActivatableStEffect(Y_DRAGON_HEAD);
	Register_StatChangeEquip(Y_DRAGON_HEAD, StatChange_Y);

	Register_ActivatableEffect(Z_METAL_TANK);
	Register_ActivatableStEffect(Z_METAL_TANK);
	Register_StatChangeEquip(Z_METAL_TANK, StatChange_Z);

	Register_AfterDamageCalculation(Protection);
	Register_OnCardLeavingField(EffectProtection);


	EffectScript scriptXYZ;
	scriptXYZ.CardID = XYZ_DRAGON_CANNON;
	scriptXYZ.Effect = 0x00585C10;
	scriptXYZ.AppliesTo = reinterpret_cast<uintptr_t>(&AppliesTo_XYZ);
	scriptXYZ.Condition = reinterpret_cast<uintptr_t>(&Condition_XYZ);
	scriptXYZ.Cost = reinterpret_cast<uintptr_t>(&Cost_XYZ);
	scriptXYZ.Target = 0x00596570;
	Register_EffectScript(scriptXYZ);

	EffectScript scriptY;
	scriptY.CardID = Y_DRAGON_HEAD;
	scriptY.Effect = reinterpret_cast<uintptr_t>(&Effect_YZ);
	scriptY.AppliesTo =reinterpret_cast<uintptr_t>(&AppliesTo_YZ);
	scriptY.Condition = reinterpret_cast<uintptr_t>(&Condition_YZ);
	scriptY.Cost = 0;
	scriptY.Target = reinterpret_cast<uintptr_t>(&Target_YZ);
	Register_EffectScript(scriptY);

	EffectScript scriptZ;
	scriptZ.CardID = Z_METAL_TANK;
	scriptZ.Effect = reinterpret_cast<uintptr_t>(&Effect_YZ);
	scriptZ.AppliesTo = reinterpret_cast<uintptr_t>(&AppliesTo_YZ);
	scriptZ.Condition = reinterpret_cast<uintptr_t>(&Condition_YZ);
	scriptZ.Cost = 0;
	scriptZ.Target = reinterpret_cast<uintptr_t>(&Target_YZ);
	Register_EffectScript(scriptZ);

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
uint32_t __cdecl Effect_YZ(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	if (funParam.finishedResolving) return 0;
	if (funParam.targetCount == 0 && funParam.location < 5) return 0;


	if (funParam.location < 5)
	{
		uint8_t tSide = funParam.GetFieldTargetSide(0);
		uint8_t tZone = funParam.GetFieldTargetZone(0);

		switch (GameData::GetEffectState())
		{
			case 0x80:
			{
				if (duel->players[tSide].monsterZones[tZone].card.GetIntID() == 0 || duel->players[tSide].monsterZones[tZone].IsFaceUp() == false) return 0;

				destZoneYZ = FUN::GetEmptySpellTrapZoneIdx(funParam.playerIdx);
				if (destZoneYZ < 0) return 0;

				FUN::MoveCardOnTheField(funParam.block, funParam.playerIdx, funParam.zoneIdx, funParam.playerIdx, destZoneYZ);
				FUN::EquipCard(funParam.playerIdx, destZoneYZ << 8 | funParam.playerIdx, tZone << 8 | funParam.playerIdx, 1);

				return 0x7f;
			}
			case 0x7f:
			{
				// Set OPT flag
				duel->players[funParam.playerIdx].monsterZones[destZoneYZ].effectIDs[31] = 0x1;

				return 0;
			}
		}
	}
	else
	{
		switch (GameData::GetEffectState())
		{
			case 0x80:
			{
				destZoneYZ = FUN::GetSummonZone(funParam.playerIdx);
				if (destZoneYZ < 0) return 0;

				uint32_t cardDWORD = duel->players[funParam.playerIdx].monsterZones[funParam.zoneIdx].card.fullValue;
				FUN::SpecialSummon(funParam.playerIdx, &cardDWORD, 1, 0x20, funParam.location, 0);
				return 0x7f;
			}	
			case 0x7f:
			{
				// Set OPT flag
				duel->players[funParam.playerIdx].monsterZones[destZoneYZ].effectIDs[31] = 0x1;

				return 0;
			}
		}
	}
}
bool __cdecl AppliesTo_YZ(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	uint8_t zoneIdx = param2 >> 8 & 0xff;
	uint8_t sideIdx = (param2 & 0xff) & 1;

	if (4 < zoneIdx) return false;
	if (sideIdx != funParam.playerIdx) return false;

	if (sideIdx == funParam.playerIdx && zoneIdx == funParam.location) return false;

	uint16_t selfID = FUN::GetCardID(funParam.cardIntID);
	uint16_t targetID = FUN::GetCardID(duel->players[sideIdx].monsterZones[zoneIdx].card.GetIntID());
	if (targetID != X_HEAD_CANNON && (targetID != Y_DRAGON_HEAD || selfID != Z_METAL_TANK)) return false;

	return true;
}
uint32_t __cdecl Condition_YZ(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	if (funParam.location > 4)
	{
		if (FUN::CanPlayerSummon(funParam.playerIdx) == 0) return 0;
		if (FUN::NumOfEmptyValidSummonZones(funParam.playerIdx) == 0) return 0;
	}
	else
	{
		if (FUN::GetEmptySpellTrapZoneIdx(funParam.playerIdx) < 0) return 0;
	}

	if (duel->players[funParam.playerIdx].monsterZones[funParam.zoneIdx].effectIDs[31] == 0x1) return 0;
	uint16_t selfID = FUN::GetCardID(funParam.cardIntID);
	for (size_t i = 0; i < 5; i++)
	{
		uint16_t cardIntID = duel->players[funParam.playerIdx].monsterZones[i].card.GetIntID();
		if (cardIntID != 0)
		{
			uint16_t cardID = FUN::GetCardID(cardIntID);
			if (cardID == X_HEAD_CANNON) return 1;
			if (cardID == Y_DRAGON_HEAD && selfID == Z_METAL_TANK) return 1;
		}
	}

	return 0;
}
uint32_t __cdecl Target_YZ(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	if (funParam.location > 4)
	{
		return 1;
	}
	else
	{
		auto targetCard = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* param, int param2, int param3)>(0x00596570);

		return targetCard(param, param2, param3);
	}
}
bool CanBeSummoned(uint32_t playerIdx)
{
	bool hasX = false;
	bool hasY = false;
	bool hasZ = false;

	for (size_t i = 0; i < 10; i++)
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
			uint8_t side = GameData::GetSelectedSide();
			uint8_t zone = GameData::GetSelectedColumn() + GameData::GetSelectedLocation();

			if (side != GameData::GetTurnPlayer() || zone > 9) return 0;

			uint16_t intID = duel->players[side].monsterZones[zone].card.GetIntID();
			if (intID == 0) return 0;

			uint16_t cardID = FUN::GetCardID(intID);
			if (!(cardID == X_HEAD_CANNON || cardID == Y_DRAGON_HEAD || cardID == Z_METAL_TANK)) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

			materials[0].zone = zone;
			materials[0].cardID = cardID;

			FUN::MarkZoneAsTributed(side, zone);

			innerState = 2;
		}break;
		case 2:
		{
			uint8_t side = GameData::GetSelectedSide();
			uint8_t zone = GameData::GetSelectedColumn() + GameData::GetSelectedLocation();

			if (side != GameData::GetTurnPlayer() || zone > 9) return 0;

			uint16_t intID = duel->players[side].monsterZones[zone].card.GetIntID();
			if (intID == 0) return 0;

			uint16_t cardID = FUN::GetCardID(intID);
			if (!(cardID == X_HEAD_CANNON || cardID == Y_DRAGON_HEAD || cardID == Z_METAL_TANK)) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;
			if (materials[0].zone == zone || materials[0].cardID == cardID) return 0;

			materials[1].zone = zone;
			materials[1].cardID = cardID;

			FUN::MarkZoneAsTributed(side, zone);

			innerState = 3;
		}break;
		case 3:
		{
			uint8_t side = GameData::GetSelectedSide();
			uint8_t zone = GameData::GetSelectedColumn() + GameData::GetSelectedLocation();

			if (side != GameData::GetTurnPlayer()) return 0;

			uint16_t intID = duel->players[side].monsterZones[zone].card.GetIntID();
			if (intID == 0) return 0;

			uint16_t cardID = FUN::GetCardID(intID);
			if (!(cardID == X_HEAD_CANNON || cardID == Y_DRAGON_HEAD || cardID == Z_METAL_TANK)) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;
			if (materials[0].zone == zone || materials[1].zone == zone) return 0;
			if (materials[0].cardID == cardID || materials[1].cardID == cardID) return 0;

			materials[2].zone = zone;
			materials[2].cardID = cardID;

			FUN::MarkZoneAsTributed(side, zone);

			FUN::FieldMaskGenerator maskGen;
			maskGen.zones[side][materials[0].zone] = true;
			maskGen.zones[side][materials[1].zone] = true;
			maskGen.zones[side][materials[2].zone] = true;

			uint8_t block[32] = {};
			FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xF, 0);
			FUN::ClearTributeMarks();

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
void __stdcall StatChange_Y(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	// Modify stats
	// 0x20 = ATK, 0x24 = DEF
	uint32_t& refATK = *(uint32_t*)(statAddress + 0x20);
	uint32_t& refDEF = *(uint32_t*)(statAddress + 0x24);

	refATK = refATK += 400;
	refDEF = refDEF += 400;
}
void __stdcall StatChange_Z(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	// Modify stats
	// 0x20 = ATK, 0x24 = DEF
	uint32_t& refATK = *(uint32_t*)(statAddress + 0x20);
	uint32_t& refDEF = *(uint32_t*)(statAddress + 0x24);

	refATK = refATK += 600;
	refDEF = refDEF += 600;

}
void __stdcall Protection()
{
	for (size_t side = 0; side < 2; side++)
	{
		if ((battleResult->sides[side].ResultFlags & 0x10) != 0)
		{
			uint8_t zone = (side == (battleResult->StateFlags & 1)) ? battleResult->GetZone(0) : battleResult->GetZone(1);

			if (duel->players[side].monsterZones[zone].effectCount != 0)
			{
				for (size_t i = 0; i < duel->players[side].monsterZones[zone].effectCount; i++)
				{
					if (duel->players[side].monsterZones[zone].effectEntries[i].type == 1)
					{
						uint16_t loc = duel->players[side].monsterZones[zone].effectIDs[i];
						uint8_t equipSide = loc & 0x1;
						uint8_t equipZone = loc >> 8 & 0xff;

						uint16_t equipIntID = duel->players[equipSide].monsterZones[equipZone].card.GetIntID();
						uint16_t equipID = FUN::GetCardID(equipIntID);
						if (equipID == Y_DRAGON_HEAD || equipID == Z_METAL_TANK)
						{
							// Prevent destruction
							battleResult->sides[side].ResultFlags &= ~0x10;

							FUN::FieldMaskGenerator maskGen;
							maskGen.zones[equipSide][equipZone] = true;
							uint8_t block[32] = {};

							FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xe, 2);
							break;
						}
					}
				}
			}
		}
	}
}
// Returns true if the rest of the stock function should be skipped, false otherwise
bool __stdcall EffectProtection(uint32_t side, uint32_t zone, uint32_t dest, uint32_t action, uint32_t effectIntID)
{
	// Check for destruction
	// Same fxCode in SendCardFromField
	if (action != 2) return false;
	if (zone > 4) return false;
	
	auto effectCount = duel->players[side].monsterZones[zone].effectCount;
	if (effectCount == 0) return false;

	for (size_t i = 0; i < effectCount; i++)
	{
		if (duel->players[side].monsterZones[zone].effectEntries[i].type == 1)
		{
			uint16_t loc = duel->players[side].monsterZones[zone].effectIDs[i];
			uint8_t equipSide = loc & 0x1;
			uint8_t equipZone = loc >> 8 & 0xff;


			uint16_t equipIntID = duel->players[equipSide].monsterZones[equipZone].card.GetIntID();
			uint16_t equipID = FUN::GetCardID(equipIntID);
			if (equipID == Y_DRAGON_HEAD || equipID == Z_METAL_TANK)
			{
				FUN::FieldMaskGenerator maskGen;
				maskGen.zones[equipSide][equipZone] = true;
				uint8_t block[32] = {};

				FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xe, 2);

				return true;
			}
		}
	}

	return false;
}