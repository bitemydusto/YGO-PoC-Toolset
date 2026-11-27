#pragma once

#include <cstdint>
#include <vector>
#include "../../Shared/Types/Types.h"
#include "../Effects/EffectCallbacks.h"

namespace YGO
{
	using CardTag = Shared::Types::CardTag;

	struct CardDefinition
	{
		uint16_t cardID;
		EffectCallbacks effect = {};
		std::vector<CardTag> tags;
	};
}
