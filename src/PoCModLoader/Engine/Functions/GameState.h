#pragma once

#include <cstdint>

namespace Engine::Functions::GameState
{
	uint32_t GetCurrentATK(uint8_t side, uint8_t zone);
	uint32_t GetCurrentDEF(uint8_t side, uint8_t zone);
}