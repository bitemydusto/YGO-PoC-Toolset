#include "CardProperty.h"
#include "../Addresses/Functions/CardProperty.h"

namespace Engine::Functions::CardProperty
{
	char* GetCardName(uint16_t intID)
	{
		return reinterpret_cast<char* (__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_NAME)(intID);
	}
	char* GetCardDescription(uint16_t intID)
	{
		return reinterpret_cast<char* (__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_DESCRIPTION)(intID);
	}
	uint32_t GetCardType(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_TYPE)(intID);
	}
	uint32_t GetCardSubType(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_SUB_TYPE)(intID);
	}
	uint32_t GetCardAttribute(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_ATTRIBUTE)(intID);
	}
	uint32_t GetCardLevel(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_LEVEL)(intID);
	}
	uint32_t GetCardAtk(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_ATK)(intID);
	}
	uint32_t GetCardDef(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_DEF)(intID);
	}
	uint32_t GetCardId(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_ID)(intID);
	}
	uint32_t GetCardIntId(uint16_t id)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_INT_ID)(id);
	}
	uint32_t GetCardStType(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_ST_TYPE)(intID);
	}
	uint32_t GetCardPack(uint16_t intID)
	{
		return reinterpret_cast<uint32_t(__cdecl*)(uint16_t)>(Addresses::Functions::CardProperty::CARD_PACK)(intID);
	}