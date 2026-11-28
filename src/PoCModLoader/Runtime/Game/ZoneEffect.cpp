#include "../../SDK/Game/ZoneEffect.h"

namespace YGO
{
	ZoneEffect::ZoneEffect(uint8_t side, uint8_t zone, uint8_t index)
	{
		this->side = side;
		this->zone = zone;
		this->index = index;
	}
}
