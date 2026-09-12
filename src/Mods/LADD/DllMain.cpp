#include <Windows.h>

#include "Utils.h"
#include "Cards.h"
#include "GameData.h"
#include "HookAPI.h"

const uint16_t LADD = Cards::ARMA_KNIGHT;

GameData::Duel duel;

void Start();

uint32_t __cdecl Effect_LADD(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_LADD(unsigned int* param, int param2, int param3);
uint32_t __cdecl Cost_LADD(unsigned int* param, int param2, int param3);

void __stdcall StatChange_LADD(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);


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
	Register_ActivatableEffect(LADD);
	Register_UnRevivable(LADD);
	Register_StatChange(LADD, StatChange_LADD);
	Register_SpellSpeed(LADD, 2);

    Utils::EffectScript script;
    script.CardID = LADD;
    script.Effect = reinterpret_cast<uintptr_t>(&Effect_LADD);
    script.AppliesTo = 0;
    script.Condition = reinterpret_cast<uintptr_t>(&Condition_LADD);
    script.Cost = reinterpret_cast<uintptr_t>(&Cost_LADD);
    script.Target = 0;

    Register_EffectScript(script);

}
uint32_t __cdecl Effect_LADD(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);

    if (funParam.finishedResolving) return 0;

    uint16_t* target = (uint16_t*)param2;

    uint16_t  loc = target[1];

    uint8_t tPlayer = loc & 1;
    uint8_t tZone = (loc >> 1) & 0x1F;

    uint32_t cmdArg0 = ((uint32_t)loc << 15) | 0xB1;
    uint32_t cmdArg1 = tZone;
    FUN::FUN_005b91e0(cmdArg0, cmdArg1, 1, 0);

    *(uint8_t*)((uint8_t*)target + 4) |= 0x0E;


    return 0;
}
uint32_t __cdecl Condition_LADD(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);

    if (param3 != 0) return 0;
	if (param2 == 0) return 0;

    uint16_t* target = (uint16_t*)param2;

    if (0x14 < (target[1] & 0x3e)) return 0;

    if ((target[1] & 0x3000) != 0) return 0;

	if (FUN::GetCardID(target[0] & 0xfff) == LADD || FUN::GetCardID(target[0] & 0xfff) == Cards::SPIRITUALISM) return 0;

	int atk = FUN::GetCurrentATK(funParam.playerIdx, funParam.zoneIdx);
	int def = FUN::GetCurrentDEF(funParam.playerIdx, funParam.zoneIdx);

	if (atk < 500 || def < 500) return 0;


    return 1;
}
uint32_t __cdecl Cost_LADD(unsigned int* param, int param2, int param3)
{
    FUN::Param funParam(param);

    uint8_t count = Utils::ReadUint8((void*)(GameData::BASE_PLAYER_ADDRESS + funParam.playerIdx * GameData::PLAYER_OFFSET + 0x10 + 0x90 * funParam.zoneIdx + 0x49));

    count++;
	Utils::WriteUint8((void*)(GameData::BASE_PLAYER_ADDRESS + funParam.playerIdx * GameData::PLAYER_OFFSET + 0x10 + 0x90 * funParam.zoneIdx + 0x49), count);

    return 1;
}
void __stdcall StatChange_LADD(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
    uint8_t count = Utils::ReadUint8((void*)(GameData::BASE_PLAYER_ADDRESS + playerIdx * GameData::PLAYER_OFFSET + 0x10 + 0x90 * zoneIdx + 0x49));
    
    // Modify stats
    // 0x20 = ATK, 0x24 = DEF
    Utils::WriteInt32((void*)(statAddress + 0x20), count * 500 * -1);
    Utils::WriteInt32((void*)(statAddress + 0x24), count * 500 * -1);
}