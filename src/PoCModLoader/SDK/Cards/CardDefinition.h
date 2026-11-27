#pragma once

#include <cstdint>
#include <vector>
#include "../../Shared/Types/Types.h"
#include "../../Shared/Structures/EffectScript.h"

namespace YGO
{
	using CardTag = Shared::Types::CardTag;

	struct CardDefinition
	{
		uint16_t cardID;
		Shared::Structures::EffectScript effectScript;
		std::vector<CardTag> tags;
	};
}
