#pragma once

#include <cstdint>

namespace Engine::Structures
{
    struct EffectScript
    {
        uint32_t cardID;
        uintptr_t effect;
        uintptr_t filter;
        uintptr_t condition;
        uintptr_t cost;
        uintptr_t target;
    };
}