#include "../../SDK/Game/Player.h"
#include "../../Engine/Structures/Duel.h"
#include "../../Engine/Addresses/Data/GameState.h"

namespace YGO
{
	Player::Player(uint8_t index)
	{
		playerIndex = index;
	}
	uint16_t Player::LP()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[playerIndex].lifePoints;
	}
	uint8_t Player::cardsInHand()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[playerIndex].cardsInHand;
	}
	uint8_t Player::cardsInDeck()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[playerIndex].cardsInDeck;
	}
	uint8_t Player::cardsInGrave()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[playerIndex].cardsInGrave;
	}
	uint8_t Player::cardsInExtra()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[playerIndex].cardsInExtra;
	}
	uint8_t Player::cardsInBanish()
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return duel->players[playerIndex].cardsInBanish;
	}
	Zone Player::zone(uint8_t index)
	{
		if (index > 10) index = 10; // Needs better handling for this
		return Zone(this->playerIndex, index);
	}
	Zone Player::monsterZone(uint8_t index)
	{
		if (index > 4) index = 4; // Needs better handling for this
		return Zone(this->playerIndex, index);
	}
	Zone Player::spellTrapZone(uint8_t index)
	{
		if (index > 4) index = 4; // Needs better handling for this
		return Zone(this->playerIndex, index + 5);
	}
	Zone Player::fieldSpellZone()
	{
		return Zone(this->playerIndex, 10);
	}
	Card Player::hand(uint8_t index)
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return Card(duel->players[playerIndex].hand[index].fullValue);
	}
	Card Player::deck(uint8_t index)
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return Card(duel->players[playerIndex].deck[index].fullValue);
	}
	Card Player::grave(uint8_t index)
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return Card(duel->players[playerIndex].grave[index].fullValue);
	}
	Card Player::extra(uint8_t index)
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return Card(duel->players[playerIndex].extra[index].fullValue);
	}
	Card Player::banish(uint8_t index)
	{
		auto duel = Engine::Structures::Duel::getDuel();
		return Card(duel->players[playerIndex].banish[index].fullValue);
	}
}