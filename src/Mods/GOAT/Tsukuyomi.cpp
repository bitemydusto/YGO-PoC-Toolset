#include "GOAT.h"

namespace
{
    constexpr uint16_t TSUKUYOMI = Cards::AIR_EATER;

    uint32_t __cdecl Effect_TSUKUYOMI(unsigned int* param, int param2, int param3);

    uint32_t __cdecl Effect_TSUKUYOMI(unsigned int* param, int param2, int param3)
    {
        FUN::Param funParam(param);

        if (funParam.finishedResolving) return 0;
        if (funParam.targetCount == 0) return 0;

        uint32_t side = funParam.GetFieldTargetSide(0);
        uint32_t zone = funParam.GetFieldTargetZone(0);

        if (duel->players[side].cardZones[zone].card.GetIntID() == 0) return 0;
        if (FUN::IsCardOnField(Cards::LIGHT_OF_INTERVENTION) != 0) return 0;

        if (duel->players[side].cardZones[zone].InAttackPosition())
        {
            FUN::ToggleMonsterPosition(side, zone, 1, 0, 0);
        }
        else FUN::ToggleFaceUp(side, zone, 0, 0);
        return 0;
    }
}

void Install_Tsukuyomi()
{
    Register_NormalSummonTrigger(TSUKUYOMI);
    Register_FlipMonster(TSUKUYOMI);
    Register_UnRevivable(TSUKUYOMI);
    Register_SpiritMonster(TSUKUYOMI);

    Register_EffectScript({
        .CardID = TSUKUYOMI,
        .Effect = reinterpret_cast<uintptr_t>(&Effect_TSUKUYOMI),
        .AppliesTo = 0x0057AD70,
        .Condition = 0,
        .Cost = 0,
        .Target = 0x00595250
        });
}