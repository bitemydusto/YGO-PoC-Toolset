#pragma once

#include "Card.h"

namespace YGO
{
	class Zone
	{
	public:
		Card* card;
		bool faceUp();
		uint32_t currentATK();
		uint32_t currentDEF();
	};
}
