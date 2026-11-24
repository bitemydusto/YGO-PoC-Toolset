#pragma once

#include <cstdint>

namespace Engine::Addresses::Data::GameState
{
	inline constexpr uintptr_t PLAYER_LIST = 0x00A55D64;
	inline constexpr uintptr_t BATTLE_RESULT = 0x00a57840;
	inline constexpr uintptr_t SELECTION_LIST_SIZE = 0x00A585A4;
	inline constexpr uintptr_t SELECTION_LIST = 0x00A582A4;
	inline constexpr uintptr_t EFFECT_SCRIPT_LIST = 0x005ed0a8;
	inline constexpr uintptr_t SUMMON_STATE = 0x00a57808;
	// Used by effect functions
	// Starts at 0x80 and changes to what the effect function returns
	// When the effect function returns 0, it is finished resolving
	inline constexpr uintptr_t EFFECT_STATE = 0x00a55c8a;
	// Used by target and cost functions
	// Starts at 0 and must be set manually
	// Make sure to set it back to 0 when the function is done
	inline constexpr uintptr_t EFFECT_SUBSTATE = 0x00a55c8e;
	inline constexpr uintptr_t LAST_CARD_USED = 0x00a57802;
	// lower byte = main state
	// upper byte = summon state
	inline constexpr uintptr_t GAME_STATE = 0x00a57808;
	inline constexpr uintptr_t SUMMON_PARAM = 0x00A55080;
	// contains the turn player in the lowest bit, and the current phase in the upper bits
	inline constexpr uintptr_t TURN_INFO = 0x00a577fa;
	// contains the local side in the lowest bit and the player type
	inline constexpr uintptr_t SIDE_INFO = 0x00A54E5C;
}