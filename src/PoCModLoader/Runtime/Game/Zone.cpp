#include "../../SDK/Game/Zone.h"
#include "../../Engine/Structures/Duel.h"
#include "../../Engine/Functions/GameState.h"
#include "../Operations/Actions.h"

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
	std::vector<ZoneEffect> Zone::effects()
	{
		std::vector<ZoneEffect> effects;
		auto duel = Engine::Structures::Duel::getDuel();
		for (size_t i = 0; i < duel->players[this->side].cardZones[this->index].effectCount; i++)
		{
			const auto type = static_cast<ZoneEffectType>(duel->players[this->side].cardZones[this->index].zoneEffects[i].type & 0x0f);
			switch (type)
			{
				case ZoneEffectType::Equip:
				{
					// Not implemented yet
				}break;
				case ZoneEffectType::Absorb:
				{
					// Not implemented yet
				}break;
				case ZoneEffectType::Permanent:
				{
					// Not implemented yet
				}break;
			}
		}

		return effects;
	}
	void Zone::addEffect(ZoneEffect* effect)
	{
		if (auto e = dynamic_cast<EquipEffect*>(effect))
		{
			return;
		}
		if (auto e = dynamic_cast<PermanentEffect*>(effect))
		{
			Runtime::Operations::Actions::AddZoneEffect(this->side, this->index, e->source->id(), (uint8_t)ZoneEffectType::Permanent);
			return;
		}
	}
}