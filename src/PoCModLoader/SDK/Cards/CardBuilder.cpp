#include "CardBuilder.h"

namespace YGO
{
	CardBuilder::CardBuilder(uint16_t cardID)
	{
		definition.cardID = cardID;
	}
	CardBuilder& CardBuilder::Effect(EffectCallbacks effect)
	{
		definition.effect = effect;
		return *this;
	}
	CardBuilder& CardBuilder::AddTag(CardTag tag)
	{
		definition.tags.push_back(tag);
		return *this;
	}
	bool CardBuilder::Build()
	{
		return false; // Placeholder for future implementation
	}
}