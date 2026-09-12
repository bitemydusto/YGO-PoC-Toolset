#include <Windows.h>

#include "Utils.h"
#include "Cards.h"
#include "GameData.h"
#include "HookAPI.h"

GameData::Duel duel;

int innerState = 0;
uint16_t monsterSummoned = 0;
uint8_t summonZone = 0;
uint8_t obeliskFirstTribute = 0;
uint8_t raEffectChoice = 0;
uint32_t raLifePaid = 0;

struct Tribute
{
    uint8_t zone;
    int atk;
    int def;
};
static Tribute tributes[3];

void Start();

uint32_t __cdecl Effect_Slifer(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_Slifer(unsigned int* param, int param2, int param3);

uint32_t __cdecl Effect_Ra(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_Ra(unsigned int* param, int param2, int param3);
uint32_t __cdecl Cost_Ra(unsigned int* param, int param2, int param3);
uint32_t __cdecl Target_Ra(unsigned int* param, int param2, int param3);

uint32_t __cdecl Condition_Obelisk(unsigned int* param, int param2, int param3);
uint32_t __cdecl Cost_Obelisk(unsigned int* param, int param2, int param3);


bool CanBeSummoned(uint32_t playerIdx);
bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col);
bool CanBeTributedObelisk(uint8_t playerIdx, uint8_t zoneIdx, uint8_t selSide, uint8_t selCol);
void __stdcall ChangeSliferStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
void __stdcall ChangeRaStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
void __stdcall SliferStatRefuce(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
uint32_t __stdcall SummonStates();

bool raCondition1(uint8_t playerIdx);
bool raCondition2(uint8_t playerIdx);


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
    // Ignore divine checks
    // ATK
    Utils::WriteBytes((void*)0x004027D0, "\xEB\x0B", 2);
    // DEF
    Utils::WriteBytes((void*)0x00402820, "\xEB\x0B", 2);
    // LVL
    Utils::WriteBytes((void*)0x004026e0, "\xEB\x0B", 2);
    // SubType
    Utils::WriteBytes((void*)0x00402758, "\x90\x90", 2);
    Utils::WriteBytes((void*)0x0040275a, "\xEB\x0D", 2);


    Register_ActivatableEffect(Cards::SLIFER_THE_SKY_DRAGON);
    Register_ActivatableEffect(Cards::OBELISK_THE_TORMENTOR);
    Register_ActivatableEffect(Cards::THE_WINGED_DRAGON_OF_RA);

    Register_NormalSummonCondition(Cards::SLIFER_THE_SKY_DRAGON, CanBeSummoned);
    Register_NormalSummonCondition(Cards::OBELISK_THE_TORMENTOR, CanBeSummoned);
    Register_NormalSummonCondition(Cards::THE_WINGED_DRAGON_OF_RA, CanBeSummoned);

    Register_InitialSummonState(Cards::SLIFER_THE_SKY_DRAGON, 0x40);
	Register_InitialSummonState(Cards::OBELISK_THE_TORMENTOR, 0x40);
	Register_InitialSummonState(Cards::THE_WINGED_DRAGON_OF_RA, 0x40);
	Register_SummonState(0x40, SummonStates);

	Register_StatChange(Cards::SLIFER_THE_SKY_DRAGON, ChangeSliferStat);
	Register_StatChange(Cards::THE_WINGED_DRAGON_OF_RA, ChangeRaStat);
	Register_StatChangeEffect(Cards::SLIFER_THE_SKY_DRAGON, SliferStatRefuce);

    Register_SpellSpeed(Cards::SLIFER_THE_SKY_DRAGON, 2);

    Utils::EffectScript scriptSlifer;
    scriptSlifer.CardID = Cards::SLIFER_THE_SKY_DRAGON;
    scriptSlifer.Effect = reinterpret_cast<uintptr_t>(&Effect_Slifer);
    scriptSlifer.AppliesTo = 0;
    scriptSlifer.Condition = reinterpret_cast<uintptr_t>(&Condition_Slifer);
    scriptSlifer.Cost = 0;
    scriptSlifer.Target = 0;

	Register_EffectScript(scriptSlifer);

    Utils::EffectScript scriptObelisk;
    scriptObelisk.CardID = Cards::OBELISK_THE_TORMENTOR;
    scriptObelisk.Effect = 0x00584C40;
    scriptObelisk.AppliesTo = 0x0057A7F0;
    scriptObelisk.Condition = reinterpret_cast<uintptr_t>(&Condition_Obelisk);
    scriptObelisk.Cost = reinterpret_cast<uintptr_t>(&Cost_Obelisk);
    scriptObelisk.Target = 0;

	Register_EffectScript(scriptObelisk);

    Utils::EffectScript scriptRa;
    scriptRa.CardID = Cards::THE_WINGED_DRAGON_OF_RA;
    scriptRa.Effect = reinterpret_cast<uintptr_t>(&Effect_Ra);
    scriptRa.AppliesTo = 0x0057A880;
    scriptRa.Condition = reinterpret_cast<uintptr_t>(&Condition_Ra);
    scriptRa.Cost = reinterpret_cast<uintptr_t>(&Cost_Ra);
    scriptRa.Target = reinterpret_cast<uintptr_t>(&Target_Ra);

	Register_EffectScript(scriptRa);

}
uint32_t __cdecl Effect_Slifer(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);
	duel = GameData::GetDuel();

    uint16_t* block = (uint16_t*)param;

    uint16_t t_zoneIdx = (block[8] >> 9) & 0xf;
    uint16_t t_playerIdx = (block[8] >> 8) & 1;

    if (funParam.finishedResolving) return 0;

	if (t_zoneIdx > 4) return 0;
    if (duel.players[t_playerIdx].monsterZones[t_zoneIdx].card.intID == 0) return 0;

    uint8_t x = Utils::ReadUint8((void*)(GameData::BASE_PLAYER_ADDRESS + GameData::PLAYER_OFFSET * t_playerIdx + 0x10 + t_zoneIdx * 0x90 + 0x6));
    if ((x & 2) == 0) return 0;

    if (FUN::GetCurrentATK(t_playerIdx, t_zoneIdx) <= 2000)
    {
        uint8_t* block8 = (uint8_t*)param;
        uint32_t y = FUN::FUN_005777D0(block8, t_playerIdx, t_zoneIdx);

        return y & 0xffff0000;

    }
    else
    {
		uint16_t packedEffectEntity = 0xb | (0 << 8);

        uint16_t effectID = FUN::GetCardIntID(Cards::SLIFER_THE_SKY_DRAGON);
		FUN::W_AddEffectEntityToZone(t_playerIdx, t_zoneIdx, effectID, packedEffectEntity);
    }

    return 0;
}
uint32_t __cdecl Condition_Slifer(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);
	duel = GameData::GetDuel();

    uint16_t* block = (uint16_t*)param;

    uint16_t t_zoneIdx = (block[8] >> 9) & 0xf;
    uint16_t t_playerIdx = (block[8] >> 8) & 1;

	if (t_zoneIdx > 4) return 0;

	uint16_t rWindow = (block[1] & 0xfc0) >> 6;
    if (rWindow != 5 && rWindow != 6 && rWindow != 7) return 0; // Normal/Flip/Special summon response window

    if ((duel.players[t_playerIdx].monsterZones[t_zoneIdx].card.intID & 0xfff) == 0) return 0;

	uint8_t x = Utils::ReadUint8((void*)(GameData::BASE_PLAYER_ADDRESS + GameData::PLAYER_OFFSET * t_playerIdx + 0x10 + t_zoneIdx * 0x90 + 0x6));
	if ((x & 2) == 0) return 0;

	if (FUN::FUN_0056C510(t_playerIdx, t_zoneIdx) == 0) return 0;

    if (t_playerIdx == funParam.playerIdx) return 0;

	return 1;
}
uint32_t __cdecl Condition_Obelisk(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	duel = GameData::GetDuel();
	GameData::Player player = duel.players[funParam.playerIdx];

    int n = 0;
	for (size_t i = 0; i < 5; i++)
	{
		if (i == funParam.zoneIdx) continue;
		if (player.monsterZones[i].card.intID != 0) n++;
	}

	if (n < 2) return 0;

	return 1;
}
uint32_t __cdecl Cost_Obelisk(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);

    uint8_t sub = GameData::GetEffectSubState();

    switch (sub)
    {
        case 0:
        {
            FUN::ShowDialog("Select @32@0 monsters to Tribute.");
            GameData::SetEffectSubState(1);
        }break;
        case 1:
        {
            if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

            uint8_t side = GameData::GetSelectedSide();
            uint8_t col = GameData::GetSelectedColumn();

            if (!CanBeTributedObelisk(funParam.playerIdx, funParam.zoneIdx, side, col)) return 0;

            if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

            FUN::MarkZoneAsTributed(side, col);

			obeliskFirstTribute = col;
            GameData::SetEffectSubState(2);
        }break;
        case 2:
        {
            if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

            uint8_t side = GameData::GetSelectedSide();
            uint8_t col = GameData::GetSelectedColumn();

            if (!CanBeTributedObelisk(funParam.playerIdx, funParam.zoneIdx, side, col)) return 0;
			if (col == obeliskFirstTribute) return 0;

            if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

            FUN::MarkZoneAsTributed(side, col);

			FUN::TributeSelected(side, obeliskFirstTribute);
            FUN::TributeSelected(side, col);

			GameData::SetEffectSubState(0);
			return 1;
        }
    }

    return 0;
}
uint32_t __cdecl Effect_Ra(unsigned int* param, int param2, int param3)
{
	if (raEffectChoice == 0)
	{
		return FUN::DestroyEffect(param, param2, param3);
	}
    else
    {
		FUN::Param funParam(param);

        if (funParam.finishedResolving) return 0;

		uint16_t atkBuff = (uint16_t)raLifePaid;

        Utils::WriteUint16((void*)(GameData::BASE_PLAYER_ADDRESS + funParam.playerIdx * GameData::PLAYER_OFFSET + 0x10 + 0x90 * funParam.zoneIdx + 0x46), atkBuff);

        return 0;
    }
}
uint32_t __cdecl Condition_Ra(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);

	if (!raCondition1(funParam.playerIdx) && !raCondition2(funParam.playerIdx)) return 0;

    return 1;
}
uint32_t __cdecl Target_Ra(unsigned int* param, int param2, int param3)
{
	if (raEffectChoice == 0)
	{
		return FUN::TargetMonster(param, param2, param3);
	}
	else
	{
		return 1;
	}
}
uint32_t __cdecl Cost_Ra(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);
    uint8_t sub = GameData::GetEffectSubState();

	switch (sub)
	{
	    case 0:
	    {
			raEffectChoice = 0;

            if (raCondition1(funParam.playerIdx) && raCondition2(funParam.playerIdx))
            {
                FUN::ShowDialog(
                    "Activate which effect?\n"
                    "\n"
                    "  Destroy (pay 1000 LP)\n"
                    "  ATK boost (pay LP to 1)\n"
                );
                FUN::SetupSelector(2, -1);
                FUN::InitiateSelector();
                GameData::SetEffectSubState(1);
            }
            else GameData::SetEffectSubState(2);
	    }break;
	    case 1:
	    {
			raEffectChoice = GameData::GetDialogResult();

            if (raEffectChoice == 0)
            {
                FUN::PayLifePoints(funParam.playerIdx, 1000);

            }
            else
            {
				raLifePaid = duel.players[funParam.playerIdx].lifePoints - 1;
				FUN::PayLifePoints(funParam.playerIdx, raLifePaid);
            }
		    GameData::SetEffectSubState(0);

            return 1;
	    }
        case 2:
        {
            raEffectChoice = 1;

            raLifePaid = duel.players[funParam.playerIdx].lifePoints - 1;
            FUN::PayLifePoints(funParam.playerIdx, raLifePaid);

            GameData::SetEffectSubState(0);

            return 1;
        }
	}

    return 0;
}
bool CanBeSummoned(uint32_t playerIdx)
{
	if (FUN::CanPlayerSummon(playerIdx) == 0) return false;
	if (FUN::NumOfEmptyValidSummonZones(playerIdx) == 0) return false;
	if (FUN::NumOfTributableMonsters(playerIdx, 0xffffffff) < 3) return false;

	return true;
}
bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col)
{
    duel = GameData::GetDuel();

    if (side != playerIdx) return false;
    if (col > 4) return false;
    if (duel.players[playerIdx].monsterZones[col].card.intID == 0) return false;

    return true;
}
bool CanBeTributedObelisk(uint8_t playerIdx, uint8_t zoneIdx, uint8_t selSide, uint8_t selCol)
{
	duel = GameData::GetDuel();
    GameData::Player player = duel.players[playerIdx];

    if (selSide != playerIdx) return false;
	if (selCol == zoneIdx) return false;
    if (selCol > 4) return false;
    if (player.monsterZones[selCol].card.intID == 0) return false;
	if (FUN::IsMonsterTributable(selSide, playerIdx, selCol) == 0) return false;

    return true;
}
bool raCondition1(uint8_t playerIdx)
{
	duel = GameData::GetDuel();
	GameData::Player player = duel.players[playerIdx];
	if (player.lifePoints <= 1000) return false;
	return true;
}
bool raCondition2(uint8_t playerIdx)
{
	duel = GameData::GetDuel();
	GameData::Player player = duel.players[playerIdx];
	if (player.lifePoints <= 1) return false;
	return true;
}
uint32_t __stdcall SummonStates()
{
    switch (innerState)
    {
        case 0:
        {
            FUN::ShowDialog("You must tribute @33@0 monsters to summon this monster. Do you wish to @2Summon@0?");
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

            if (!CanBeTributed(1, side, col)) return 0;

            if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

            FUN::MarkZoneAsTributed(side, col);

			duel = GameData::GetDuel();

			tributes[0].zone = col;
			tributes[0].atk = FUN::GetCurrentATK(side, col);
			tributes[0].def = FUN::GetCurrentDEF(side, col);

            innerState = 3;
        }break;
        case 3:
        {
            if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

            uint8_t side = GameData::GetSelectedSide();
            uint8_t col = GameData::GetSelectedColumn();

            if (col == tributes[0].zone) return 0;
            if (!CanBeTributed(1, side, col)) return 0;

            if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

            FUN::MarkZoneAsTributed(side, col);

            duel = GameData::GetDuel();

            tributes[1].zone = col;
            tributes[1].atk = FUN::GetCurrentATK(side, col);
            tributes[1].def = FUN::GetCurrentDEF(side, col);

            innerState = 4;
        }break;
        case 4:
        {
            if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

            uint8_t side = GameData::GetSelectedSide();
            uint8_t col = GameData::GetSelectedColumn();

            if (col == tributes[0].zone) return 0;
            if (col == tributes[1].zone) return 0;
            if (!CanBeTributed(1, side, col)) return 0;

            if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

            FUN::MarkZoneAsTributed(side, col);

            duel = GameData::GetDuel();

            tributes[2].zone = col;
            tributes[2].atk = FUN::GetCurrentATK(side, col);
            tributes[2].def = FUN::GetCurrentDEF(side, col);

            FUN::TributeSelected(side, tributes[0].zone);
			FUN::TributeSelected(side, tributes[1].zone);
            FUN::TributeSelected(side, tributes[2].zone);

            innerState = 5;
        }break;
        case 5:
        {
            uint16_t sel = GameData::GetSelectedSoFar();
            GameData::SetSelectedSoFar(sel & 0xff00);

            uint32_t handIdx = Utils::ReadUint8((void*)0x00a5780c);
            uint32_t zone = FUN::GetSummonZone(1);
            uint16_t choice = (Utils::ReadUint8((void*)0x00a57804) >> 3) & 1;
			uint8_t set = choice == 0 ? 1 : 0;

            monsterSummoned = GameData::GetCardUsed();
			summonZone = zone;

            FUN::NormalSummon(1, handIdx, zone, 0, set);

            innerState = 6;
        }break;
        case 6:
        {
			uint16_t id = FUN::GetCardID(monsterSummoned);
            if (id == Cards::THE_WINGED_DRAGON_OF_RA)
            {
                // Write atk buff
				int atkBuff = tributes[0].atk + tributes[1].atk + tributes[2].atk;
                atkBuff = atkBuff / 50;
				if (atkBuff > 255) atkBuff = 255;
                Utils::WriteUint8((void*)(GameData::BASE_PLAYER_ADDRESS + 1 * GameData::PLAYER_OFFSET + 0x10 + 0x90 *  summonZone + 0x48), (uint8_t)atkBuff);
				// Write def buff
				int defBuff = tributes[0].def + tributes[1].def + tributes[2].def;
				defBuff = defBuff / 50;
				if (defBuff > 255) defBuff = 255;
				Utils::WriteUint8((void*)(GameData::BASE_PLAYER_ADDRESS + 1 * GameData::PLAYER_OFFSET + 0x10 + 0x90 * summonZone + 0x49), (uint8_t)defBuff);
            }
            uint32_t x = Utils::ReadUint8((void*)0x00a57804);
            Utils::WriteUint32((void*)0x00a57804, x & 0xfffffffd);
			FUN::FUN_00579880(1, 0x11, 1); // Set already normal summoned this turn flag

			innerState = 0;
            return 1;
        }

    }

    return 0;
}
void __stdcall ChangeSliferStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	duel = GameData::GetDuel();
	GameData::Player player = duel.players[playerIdx];

    // Modify stats
    // 0x20 = ATK, 0x24 = DEF
    Utils::WriteInt32((void*)(statAddress + 0x20), player.cardsInHand * 1000);
    Utils::WriteInt32((void*)(statAddress + 0x24), player.cardsInHand * 1000);

}
void __stdcall SliferStatRefuce(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	duel = GameData::GetDuel();
	GameData::Player player = duel.players[playerIdx];
	// Modify stats
	// 0x20 = ATK, 0x24 = DEF
	uint32_t currentATK = Utils::ReadUint32((void*)(statAddress + 0x20));

	Utils::WriteInt32((void*)(statAddress + 0x20), currentATK - 2000);
}
void __stdcall ChangeRaStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	uint8_t atkBuff = Utils::ReadUint8((void*)(GameData::BASE_PLAYER_ADDRESS + playerIdx * GameData::PLAYER_OFFSET + 0x10 + 0x90 * zoneIdx + 0x48));
	uint8_t defBuff = Utils::ReadUint8((void*)(GameData::BASE_PLAYER_ADDRESS + playerIdx * GameData::PLAYER_OFFSET + 0x10 + 0x90 * zoneIdx + 0x49));
	uint16_t lpBuff = Utils::ReadUint16((void*)(GameData::BASE_PLAYER_ADDRESS + playerIdx * GameData::PLAYER_OFFSET + 0x10 + 0x90 * zoneIdx + 0x46));

    // Modify stats
    // 0x20 = ATK, 0x24 = DEF
    Utils::WriteInt32((void*)(statAddress + 0x20), atkBuff * 50 + lpBuff);
    Utils::WriteInt32((void*)(statAddress + 0x24), defBuff * 50);
}