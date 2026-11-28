#include "../../SDK/Game/Zone.h"
#include "../../Engine/Structures/Duel.h"
#include "../../Engine/Functions/GameState.h"

namespace YGO
{
	Zone::Zone(uint8_t side, uint8_t index)
	{
		this->side = side;
		this->index = index;
	}

	Card Zone::card()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return Card(duel->players[this->side].cardZones[this->index].card.fullValue);
	}
	bool Zone::faceUp()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[this->side].cardZones[this->index].IsFaceUp();
	}
	uint32_t Zone::currentATK()
	{
		return Engine::Functions::GameState::GetCurrentATK(this->side, this->index);
	}
	uint32_t Zone::currentDEF()
	{
		return Engine::Functions::GameState::GetCurrentDEF(this->side, this->index);
	}
	Zone::Position Zone::position()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[this->side].cardZones[this->index].InAttackPosition() ? Zone::Position::Attack : Zone::Position::Defense;
	}
	uint8_t Zone::turnCounters()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[this->side].cardZones[this->index].GetTurnCounter();
	}
	uint8_t Zone::deathCounters()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[this->side].cardZones[this->index].GetDeathCounter();
	}
	uint16_t Zone::effectCount()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[this->side].cardZones[this->index].effectCount;
	}
	ZoneEffect Zone::effect(uint8_t index)
	{
		auto duel = Engine::Structures::Duel::getDuel();
		if (index >= duel->players[this->side].cardZones[this->index].effectCount) throw;
		
		return ZoneEffect(this->side, this->index, index);
	}
}