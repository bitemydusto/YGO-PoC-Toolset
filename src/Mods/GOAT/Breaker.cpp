#include "GOAT.h"

namespace
{
    constexpr uint16_t BREAKER = Cards::RHAIMUNDOS_OF_THE_RED_SWORD;

    uint32_t __cdecl Effect_Breaker(unsigned int* self, unsigned int* source, int mode);
    uint32_t __cdecl AppliesTo_Breaker(unsigned int* self, unsigned int* source, int mode);
    uint32_t __cdecl Condition_Breaker(unsigned int* self, unsigned int* source, int mode);
    uint32_t __cdecl Cost_Breaker(unsigned int* self, unsigned int* source, int mode);
    uint32_t __cdecl Target_Breaker(unsigned int* self, unsigned int* source, int mode);

    void __stdcall ChangeStat(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

    uint32_t __cdecl Effect_Breaker(unsigned int* self, unsigned int* source, int mode)
    {
        FUN::Param selfParam(self);

        if (selfParam.responseWindow != 5)
        {
            uint32_t result = FUN::DestroyEffect(self, source, mode);

            return result;

        }

        AddSpellCounter(selfParam.playerIdx, selfParam.zoneIdx);
        return 0;
    }
    uint32_t __cdecl AppliesTo_Breaker(unsigned int* self, unsigned int* source, int mode)
    {
        FUN::Param selfParam(self);

        if (selfParam.responseWindow != 5)
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
        uint16_t t_zoneIdx = (selfParam.block16[8] >> 9) & 0xf;
        uint16_t t_playerIdx = (selfParam.block16[8] >> 8) & 1;

        uint32_t selfInst = duel->players[selfParam.playerIdx].cardZones[selfParam.zoneIdx].card.GetInstance();
        uint32_t summonInst = duel->players[t_playerIdx].cardZones[t_zoneIdx].card.GetInstance();

        return selfInst == summonInst ? 1 : 0;
    }
    uint32_t __cdecl Cost_Breaker(unsigned int* self, unsigned int* source, int mode)
    {
        FUN::Param selfParam(self);

        if (selfParam.responseWindow != 5)
        {
            RemoveSpellCounter(selfParam.playerIdx, selfParam.zoneIdx);
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
}

void Install_Breaker()
{
	Register_StatChange(BREAKER, ChangeStat);
	Register_ActivatableEffect(BREAKER);
	Register_NormalSummonTrigger(BREAKER);

	Register_EffectScript({
		.CardID = BREAKER,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_Breaker),
		.AppliesTo = reinterpret_cast<uintptr_t>(&AppliesTo_Breaker),
		.Condition = reinterpret_cast<uintptr_t>(&Condition_Breaker),
		.Cost = reinterpret_cast<uintptr_t>(&Cost_Breaker),
		.Target = reinterpret_cast<uintptr_t>(&Target_Breaker)
		});
}
