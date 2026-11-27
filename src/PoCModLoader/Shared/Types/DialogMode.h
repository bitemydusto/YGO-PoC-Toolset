#pragma once


#include <cstdint>

namespace Shared::Types
{
	enum class DialogMode : uint8_t
	{
		OK = 0,
		YESNO = 1,
		OPTION2 = 2,
		OPTION3 = 3,
		TYPE = 4,
		ATTRIBUTE = 5,
		POSITION = 6,
		COIN = 7,
	};
}