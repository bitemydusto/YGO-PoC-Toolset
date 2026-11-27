#include "../../SDK/Game/Actions.h"
#include "../../Engine/Functions/Action.h"

namespace YGO
{
	void Draw(uint8_t player, uint8_t count)
	{
		Engine::Functions::Action::Draw(player, count);
	}
}