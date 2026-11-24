#pragma once

#include <cstdint>

namespace Engine::Addresses::Functions::Action
{
	inline constexpr uintptr_t PAY_LP = 0x005783b0;
	inline constexpr uintptr_t SEND_CARD_FROM_FIELD = 0x005768b0;
	inline constexpr uintptr_t DISCARD_FROM_HAND = 0x005758c0;
	inline constexpr uintptr_t DEAL_EFFECT_DAMAGE = 0x00578430;
	inline constexpr uintptr_t BANISH_FROM_GRAVE = 0x00575f30;
	inline constexpr uintptr_t SUMMON = 0x005ad890;
	inline constexpr uintptr_t NORMAL_SUMMON = 0x005ad710;
	inline constexpr uintptr_t MARK_ZONE_FOR_TRIBUTE = 0x00486bb0;
	inline constexpr uintptr_t TRIBUTE_ZONE = 0x00577a80;
	inline constexpr uintptr_t PAY_CHAIN_ENERGY = 0x0059cfe0;
}