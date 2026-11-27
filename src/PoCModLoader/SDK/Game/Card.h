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
		uint16_t id();
		uint8_t level();
		Attribute attribute();
		CardSubType subType();
		CardType cardType();

		uint32_t defaultATK();
		uint32_t defaultDEF();

		std::vector<CardTag> tags();
	private:
		Shared::Types::Location location;
		uint8_t side;
		uint8_t zone;
	};
}