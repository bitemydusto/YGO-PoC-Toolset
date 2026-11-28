#include "GameState.h"
#include "../Addresses/Functions/GameState.h"

namespace Engine::Functions::GameState
{
	uint32_t GetCurrentATK(uint8_t side, uint8_t zone)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint8_t, uint8_t)>(Addresses::Functions::GameState::GET_CURRECT_ATK)(side, zone);
	}
	uint32_t GetCurrentDEF(uint8_t side, uint8_t zone)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint8_t, uint8_t)>(Addresses::Functions::GameState::GET_CURRECT_DEF)(side, zone);
	}
}