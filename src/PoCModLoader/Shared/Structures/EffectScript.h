#pragma once

#include <cstdint>
#include "../Types/Callbacks.h"

namespace Shared::Structures
{
    struct EffectScript
    {
        uint32_t cardID;
        Shared::Types::EffectCallback effect = nullptr;
        Shared::Types::FilterCallback filter = nullptr;
        Shared::Types::ConditionCallback condition = nullptr;
        Shared::Types::CostCallback cost = nullptr;
        Shared::Types::TargetCallback target = nullptr;
    };
}