#include "Actions.h"

namespace Runtime::Operations::Actions
{
	void AddZoneEffect(uint8_t side, uint8_t zone, uint16_t cardId, uint16_t effectPack)
	{
		uint16_t cardIntId = Engine::Functions::CardProperty::GetCardIntId(cardId);
		Engine::Functions::Action::AddEffectEntityToZone((side << 8) | zone, cardIntId, effectPack);
	}
}