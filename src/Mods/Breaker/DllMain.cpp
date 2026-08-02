#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"

using DestroyEffect_t = uint32_t(__cdecl*)(uint32_t paramAddress, int param2, int param3);
inline DestroyEffect_t DestroyEffect = reinterpret_cast<DestroyEffect_t>(0x00585c10);

Utils::Hook hBreaker;

void Start();

uint32_t __cdecl Effect_Breaker(uint32_t paramAddress, int param2, int param3);
uint32_t __cdecl Condition_Breaker(uint32_t paramAddress, int param2, int param3);
uint32_t __cdecl Cost_Breaker(uint32_t paramAddress, int param2, int param3);

void __stdcall ChangeStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);


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
	Register_StateChange(0x96, ChangeStat);


    GameData::EffectScript script;
    script.CardID = 0x96;
    script.Effect = reinterpret_cast<uintptr_t>(&Effect_Breaker);
    script.AppliesTo = 0x0057B4A0;
    script.Condition = reinterpret_cast<uintptr_t>(&Condition_Breaker);
    script.Cost = reinterpret_cast<uintptr_t>(&Cost_Breaker);
    script.Target = 0x005959D0;

    int idx = GameData::GetEffectScriptIndex(0x96);
    GameData::SetEffectScript(idx, script);

}
uint32_t __cdecl Effect_Breaker(uint32_t paramAddress, int param2, int param3)
{
    // Original in-game function
	uint32_t result = DestroyEffect(paramAddress, param2, param3);


	return result;
}
uint32_t __cdecl Condition_Breaker(uint32_t paramAddress, int param2, int param3)
{
    uint8_t zoneIdx = (Utils::ReadUint8((void*)(paramAddress + 0x2)) >> 1) & 0xF;
    uint8_t playerIdx = Utils::ReadUint8((void*)(paramAddress + 0x2)) &  0x1;

    if ((Utils::ReadUint8((void*)(0x00a55d64 + 0xD44 * playerIdx + 0x10 + 0x90 * zoneIdx + 0x48)) & 0x1) == 0x1) return 0;
 
    return 1;
}
uint32_t __cdecl Cost_Breaker(uint32_t paramAddress, int param2, int param3)
{
    // Set flag
    uint8_t zoneIdx = (Utils::ReadUint8((void*)(paramAddress + 0x2)) >> 1) & 0xF;
    uint8_t playerIdx = Utils::ReadUint8((void*)(paramAddress + 0x2)) &  0x1;

    Utils::WriteUint16((void*)(0x00a55d64 + 0xD44 * playerIdx + 0x10 + 0x90 * zoneIdx + 0x48), 0x1);

    return 1;
}
void __stdcall ChangeStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	// Check Breaker's unique flag to see if the stat change should be applied
    if (Utils::ReadUint8((void*)(0x00a55d64 + 0xD44 * playerIdx + 0x10 + 0x90 * zoneIdx + 0x48)) == 0x1)
    {
        // Modify stats
		// 0x20 = ATK, 0x24 = DEF
        Utils::WriteInt32((void*)(statAddress + 0x20), -300);
    }
}