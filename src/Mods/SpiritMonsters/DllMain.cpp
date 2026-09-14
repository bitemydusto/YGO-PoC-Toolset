#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

const uint16_t TSUKUYOMI = Cards::AIR_EATER;

GameData::Duel duel;

uint32_t __cdecl Effect_TSUKUYOMI(unsigned int* param, int param2, int param3);

void Start();

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
    Register_ActivatableEffect(TSUKUYOMI);
    Register_UnRevivable(TSUKUYOMI);
	Register_SpiritMonster(TSUKUYOMI);

    Utils::EffectScript script;
    script.CardID = TSUKUYOMI;
    script.Effect = reinterpret_cast<uintptr_t>(&Effect_TSUKUYOMI);
    script.AppliesTo = 0x0057AD70; // Face-up only
    script.Condition = 0;
    script.Cost = 0;
    script.Target = 0x00595250; // Target

    Register_EffectScript(script);

}
uint32_t __cdecl Effect_TSUKUYOMI(unsigned int* param, int param2, int param3)
{
	FUN::Param funParam(param);

	if (funParam.finishedResolving) return 0;
	if (funParam.targetCount == 0) return 0;

    uint32_t side = funParam.GetFieldTargetSide(0);
    uint32_t zone = funParam.GetFieldTargetZone(0);

    if (GameData::GetDuel().players[side].monsterZones[zone].card.intID == 0) return 0;

	FUN::ChangeMonsterPosition(funParam.block, side, zone, 1, 0);
	return 0;
}
