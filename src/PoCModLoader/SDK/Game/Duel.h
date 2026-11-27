#pragma once

#include "Player.h"

namespace YGO
{
	class Duel
	{
	public:
		static uint8_t turnPlayer();
		static uint8_t currentPhase();
		static Player getPlayer(uint8_t index);
	};
}