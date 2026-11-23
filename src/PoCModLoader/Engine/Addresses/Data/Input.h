#pragma once

#include <cstdint>

namespace Addresses::Data::Input
{
	inline constexpr uintptr_t SELECTED_SIDE = 0x000a55044;
	// 0x0 : Monster Zone
	// 0x5 : Spell/Trap Zone
	// 0xa : Field Zone
	// 0xb : Hand
	// 0xc : Extra
	// 0xd : Deck
	// 0xe : Graveyard
	// 0xf : Banish
	inline constexpr uintptr_t SELECTED_LOCATION = 0x00a55048;
	inline constexpr uintptr_t SELECTED_COLUMN = 0x00a5504c;
	inline constexpr uintptr_t SELECTED_FLAG = 0x00a5780a;
	inline constexpr uintptr_t CONFIRMED_HAND_IDX = 0x00a55064;
	inline constexpr uintptr_t DLG_RESULT = 0x00a558b4;
}
