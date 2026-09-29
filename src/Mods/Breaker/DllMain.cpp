#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"

const uint16_t BREAKER = 0x96;

void Start();

uint32_t __cdecl Effect_Breaker(unsigned int* self, unsigned int* source, int mode);
uint32_t __cdecl AppliesTo_Breaker(unsigned int* self, unsigned int* source, int mode);
uint32_t __cdecl Condition_Breaker(unsigned int* self, unsigned int* source, int mode);
uint32_t __cdecl Cost_Breaker(unsigned int* self, unsigned int* source, int mode);
uint32_t __cdecl Target_Breaker(unsigned int* self, unsigned int* source, int mode);

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
	Register_StatChange(BREAKER, ChangeStat);
	Register_ActivatableEffect(BREAKER);
	Register_NormalSummonTrigger(BREAKER);


    Utils::EffectScript script;
    script.CardID = BREAKER;
    script.Effect = reinterpret_cast<uintptr_t>(&Effect_Breaker);
    script.AppliesTo = reinterpret_cast<uintptr_t>(&AppliesTo_Breaker);
    script.Condition = reinterpret_cast<uintptr_t>(&Condition_Breaker);
    script.Cost = reinterpret_cast<uintptr_t>(&Cost_Breaker);
    script.Target = reinterpret_cast<uintptr_t>(&Target_Breaker);

    Register_EffectScript(script);


}
uint32_t __cdecl Effect_Breaker(unsigned int* self, unsigned int* source, int mode)
{
	FUN::Param selfParam(self);

    if (selfParam.responseWindow < 5 || selfParam.responseWindow > 7)
    {
        uint32_t result = FUN::DestroyEffect(self, source, mode);

        return result;

    }
    else
    {
		AddSpellCounter(selfParam.playerIdx, selfParam.zoneIdx);

        return 0;
    }
}
uint32_t __cdecl AppliesTo_Breaker(unsigned int* self, unsigned int* source, int mode)
{
    FUN::Param selfParam(self);

    if (selfParam.responseWindow < 5 || selfParam.responseWindow > 7)
    {
		auto SpellCards = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* param, unsigned int* param2, int param3)>(0x0057B4A0);

		return SpellCards(self, source, mode);
    }

    return 1;
}
uint32_t __cdecl Condition_Breaker(unsigned int* self, unsigned int* source, int mode)
{
    FUN::Param selfParam(self);

    if (selfParam.responseWindow != 5)
    {
		uint8_t n = GetSpellCounters(selfParam.playerIdx, selfParam.zoneIdx);

		return n > 0 ? 1 : 0;
    }

    return 1;
}
uint32_t __cdecl Cost_Breaker(unsigned int* self, unsigned int* source, int mode)
{
    FUN::Param selfParam(self);

    if (selfParam.responseWindow != 5)
    {
		RemoveSpellCounter(selfParam.playerIdx, selfParam.zoneIdx);

        return 1;
    }

    return 1;
}
uint32_t __cdecl Target_Breaker(unsigned int* self, unsigned int* source, int mode)
{
	FUN::Param selfParam(self);

	if (selfParam.responseWindow != 5)
	{
		auto TargetCard = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* param, unsigned int* param2, int param3)>(0x005959D0);

		return TargetCard(self, source, mode);
	}
	return 1;
}
void __stdcall ChangeStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
    int count = GetSpellCounters(playerIdx, zoneIdx);
    if (count == 0) return;

    // Modify stats
    // 0x20 = ATK, 0x24 = DEF
    uint32_t& refATK = *(uint32_t*)(statAddress + 0x20);

    refATK = refATK += 300 * count;

}