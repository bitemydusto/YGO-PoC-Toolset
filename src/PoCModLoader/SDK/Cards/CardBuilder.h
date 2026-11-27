#pragma once

#include <cstdint>
#include "CardDefinition.h"
#include "../Effects/EffectCallbacks.h"

namespace YGO
{
	class CardBuilder
	{
	public:
		explicit CardBuilder(uint16_t cardID);

		CardBuilder& Effect(EffectCallbacks effect);
		CardBuilder& AddTag(CardTag tag);

		bool Build();

	private:
		CardDefinition definition;
	};
}
