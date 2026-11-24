#pragma once

#include <cstdint>

namespace Engine::Structures
{
    struct EffectScript
    {
        uint32_t CardID;
        uintptr_t Effect;
        uintptr_t Filter;
        uintptr_t Condition;
        uintptr_t Cost;
        uintptr_t Target;
    };
}