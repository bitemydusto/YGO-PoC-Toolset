#pragma once

#include <cstdint>

namespace Shared::Types
{
	enum class CardType : uint8_t
	{
		Dragon = 0x01,
		Zombie = 0x02,
		Fiend = 0x03,
		Pyro = 0x04,
		SeaSerpent = 0x05,
		Rock = 0x06,
		Machine = 0x07,
		Fish = 0x08,
		Dinosaur = 0x09,
		Insect = 0x0A,
		Beast = 0x0B,
		BeastWarrior = 0x0C,
		Plant = 0x0D,
		Aqua = 0x0E,
		Warrior = 0x0F,
		WingedBeast = 0x10,
		Fairy = 0x11,
		Spellcaster = 0x12,
		Thunder = 0x13,
		Reptile = 0x14,
		Trap = 0x15,
		Spell = 0x16
	};
}
