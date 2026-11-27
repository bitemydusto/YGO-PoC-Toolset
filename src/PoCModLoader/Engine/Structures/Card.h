#pragma once

#include <cstdint>

namespace Shared::Structures
{
	struct Card
	{
		uint32_t fullValue;

		uint16_t GetIntID()
		{
			return fullValue & 0xfff;
		}
		uint8_t GetOwner()
		{
			return (fullValue >> 12) & 1;
		}
		uint32_t GetInstance()
		{
			return (fullValue >> 12 & 1) + (fullValue >> 24 & 0x7F) * 2;
		}
		bool WasProperlySummoned()
		{
			return (fullValue >> 0xe & 1) != 0;
		}
		uint32_t GetPack(uint8_t side, uint8_t zone)
		{
			return ((uint32_t)(zone & 0x1F) | ((uint32_t)side << 0xf) | 0x0A20u) << 16 | GetIntID();
		}
	};
}
