#include "../../SDK/Game/Card.h"
#include "../../Engine/Functions/CardProperty.h"
#include "../../Shared/Types/Types.h"

namespace YGO
{
	Card::Card(uint32_t cardDWORD)
	{
		this->intId = cardDWORD & 0xfff;
	}

	uint16_t Card::id()
	{
		return Engine::Functions::CardProperty::GetCardId(this->intId);
	}
	const char* Card::name()
	{
		return Engine::Functions::CardProperty::GetCardName(this->intId);
	}
	const char* Card::description()
	{
		return Engine::Functions::CardProperty::GetCardDescription(this->intId);
	}
	uint8_t Card::level()
	{
		return Engine::Functions::CardProperty::GetCardLevel(this->intId);
	}
	Card::Attribute Card::attribute()
	{
		return Card::Attribute(Engine::Functions::CardProperty::GetCardAttribute(this->intId));
	}
	Card::CardSubType Card::subType()
	{
		return Card::CardSubType(Engine::Functions::CardProperty::GetCardSubType(this->intId));
	}
	Card::CardType Card::cardType()
	{
		return Card::CardType(Engine::Functions::CardProperty::GetCardType(this->intId));
	}
	uint32_t Card::defaultATK()
	{
		return Engine::Functions::CardProperty::GetCardAtk(this->intId);
	}
	uint32_t Card::defaultDEF()
	{
		return Engine::Functions::CardProperty::GetCardDef(this->intId);
	}
}