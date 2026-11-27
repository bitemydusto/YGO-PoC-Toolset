#pragma once

#include "../../SDK/Game/Duel.h"
#include "../../Engine/Addresses/Data/GameState.h"

namespace YGO
{
	uint8_t Duel::turnPlayer()
	{
		auto turnInfo = Engine::Addresses::Data::GameState::TURN_INFO;
		return *(uint8_t*)turnInfo & 1;
	}
	uint8_t Duel::currentPhase()
	{
		auto turnInfo = Engine::Addresses::Data::GameState::TURN_INFO;
		return *(uint8_t*)turnInfo >> 1;
	}
	Player* Duel::getPlayer(uint8_t index)
	{
		return new Player(index);
	}
}