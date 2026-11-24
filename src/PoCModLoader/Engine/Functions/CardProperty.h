#pragma once

#include <cstdint>

namespace Engine::Functions::CardProperty
{
	char* GetCardName(uint16_t intID);
	char* GetCardDescription(uint16_t intID);
	uint32_t GetCardType(uint16_t intID);
	uint32_t GetCardSubType(uint16_t intID);
	uint32_t GetCardAttribute(uint16_t intID);
	uint32_t GetCardLevel(uint16_t intID);
	uint32_t GetCardAtk(uint16_t intID);
	uint32_t GetCardDef(uint16_t intID);
	uint32_t GetCardId(uint16_t intID);
	uint32_t GetCardIntId(uint16_t id);
	uint32_t GetCardStType(uint16_t intID);
	uint32_t GetCardPack(uint16_t intID);
}