#pragma once

#include <cstdint>

namespace Engine::Types
{
	enum class ResponseWindow : uint8_t
	{
		TURN_START = 0x1,
		TURN_END = 0x3,
		NORMAL_SUMMON = 0x5,
		FLIP_SUMMON = 0x6,
		SPECIAL_SUMMON = 0x7,
		SET_MONSTER = 0x8,
		ATTACK_DECLARATION = 0x12,
		ENTER_MAIN2 = 0x16,
		SUMMON_ATTEMPT = 0x20,
		TRAP_ACTIVATION = 0x21,
		SPELL_ACTIVATION = 0x22,
		DRAW_CARD = 0x1b,
		FIELD_DRESTORY = 0x1c,
		DISCARD = 0x1e,
		MILL = 0x1d
	};
}