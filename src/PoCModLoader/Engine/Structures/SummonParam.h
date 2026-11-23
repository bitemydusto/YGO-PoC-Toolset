#pragma once

#include <cstdint>

namespace Structures
{
	struct SummonParam {
		uint8_t  side;
		uint8_t  destZone;
		uint8_t  srcPlace;
		uint8_t  srcIndex;
		bool     faceUp;
		bool     atkPos;
		uint8_t  tribZone[3];
		uint8_t  tribSide[3];
		bool     tribUsed[3];

		SummonParam()
		{
			uint32_t p = *(uint32_t*)0x00A55080;

			side = p & 1;
			destZone = (p >> 1) & 0x1F;
			uint32_t src = (p >> 6) & 0xFF;
			srcIndex = src & 0xF;
			srcPlace = src >> 4;
			faceUp = (p & 0x4000) != 0;
			atkPos = (p & 0x8000) != 0;
			tribZone[0] = (p >> 16) & 7;
			tribZone[1] = (p >> 19) & 7;
			tribZone[2] = (p >> 22) & 7;
			tribUsed[0] = (p & 0x02000000) != 0;
			tribUsed[1] = (p & 0x04000000) != 0;
			tribUsed[2] = (p & 0x08000000) != 0;
			tribSide[0] = (p >> 28) & 1;
			tribSide[1] = (p >> 29) & 1;
			tribSide[2] = (p >> 30) & 1;
		}
	};
}
