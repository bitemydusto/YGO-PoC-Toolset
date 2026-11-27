#pragma once

#include <cstdint>
#include "../../Shared/Structures/EffectBlock.h"

namespace Shared::Types
{
	using EffectBlock = Shared::Structures::EffectBlock;

	using EffectCallback = uint32_t(__cdecl*)(EffectBlock* self, EffectBlock* source, int mode);
	using FilterCallback = bool(__cdecl*)(EffectBlock* self, EffectBlock* source, int mode);
	using ConditionCallback = EffectCallback;
	using CostCallback = EffectCallback;
	using TargetCallback = EffectCallback;
}
