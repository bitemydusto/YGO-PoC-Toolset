#pragma once

#include <cstdint>

namespace FUN
{
    using GetMonsterType_t = uint32_t(__cdecl*)(uint16_t);
    inline GetMonsterType_t GetMonsterType = reinterpret_cast<GetMonsterType_t>(0x004025D0);

    using GetMonsterAttribute_t = uint32_t(__cdecl*)(uint16_t);
    inline GetMonsterAttribute_t GetMonsterAttribute = reinterpret_cast<GetMonsterAttribute_t>(0x00402650);

    using ShowDialogue_t = void(__cdecl*)(const char*);
    inline ShowDialogue_t ShowDialogue = reinterpret_cast<ShowDialogue_t>(0x005bf860);

	using PayLifePoints_t = void(__cdecl*)(uint32_t playerIdx, uint32_t amount);
    inline PayLifePoints_t PayLifePoints = reinterpret_cast<PayLifePoints_t>(0x005783b0);
}