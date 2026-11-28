#pragma once

#include <cstdint>
#include "../../Engine/Functions/Action.h"
#include "../../Engine/Functions/CardProperty.h"

namespace Runtime::Operations::Actions
{
	void AddZoneEffect(uint8_t side, uint8_t zone, uint16_t cardId, uint16_t effectPack);
}
