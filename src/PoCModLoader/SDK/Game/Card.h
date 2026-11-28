#include <cstdint>
#include <vector>
#include "../../Shared/Types/Types.h"

namespace YGO
{
	class Card
	{
		using Location = Shared::Types::Location;
		using Attribute = Shared::Types::Attribute;
		using CardType = Shared::Types::CardType;
		using CardSubType = Shared::Types::CardSubType;
		using CardTag = Shared::Types::CardTag;

	public:
		Card(uint32_t cardDWORD);

		uint16_t id();
		const char* name();
		const char* description();
		uint8_t level();
		Attribute attribute();
		CardSubType subType();
		CardType cardType();

		uint32_t defaultATK();
		uint32_t defaultDEF();

		std::vector<CardTag> tags();
	private:
		uint32_t intId;
	};
}