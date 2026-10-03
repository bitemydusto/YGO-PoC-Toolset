#include "GOAT.h"

namespace
{
	constexpr uint16_t SAKURETSU_ARMOR = 0x803;

	uint32_t inst = 0;

	uint32_t __cdecl Effect_Sakuretsu(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.finishedResolving) return 0;
		if (selfParam.targetCount == 0) return 0;

		auto targetCard = duel->players[selfParam.GetFieldTargetSide(0)].cardZones[selfParam.GetFieldTargetZone(0)].card;

		if (targetCard.GetIntID() == 0) return 0;
		if (inst != targetCard.GetInstance()) return 0;

		FUN::FieldMaskGenerator maskGen;
		maskGen.zones[selfParam.GetFieldTargetSide(0)][selfParam.GetFieldTargetZone(0)] = true;

		FUN::SendCardFromField(selfParam.block, maskGen.GenerateMask(), 0xe, 2);

		return 0;
	}
	uint32_t __cdecl Condition_Sakuretsu(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.responseWindow != ResponseWindow::ATTACK_DECLARATION) return 0;
		if (selfParam.playerIdx == selfParam.triggerSide) return 0;
		if (FUN::CanCardBeTargeted(selfParam.triggerSide, selfParam.triggerZone) == 0) return 0;

		return 1;
	}
	uint32_t __cdecl Target_Sakuretsu(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);
		inst = duel->players[selfParam.triggerSide].cardZones[selfParam.triggerZone].card.GetInstance();

		return FUN::TargetCard(self, selfParam.triggerSide, selfParam.triggerZone);
	}
}

void Install_SakuretsuArmor()
{
	Register_EffectScript({
		.CardID = SAKURETSU_ARMOR,
		.Effect = reinterpret_cast<uintptr_t>(Effect_Sakuretsu),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(Condition_Sakuretsu),
		.Cost = 0,
		.Target = reinterpret_cast<uintptr_t>(Target_Sakuretsu)
		});
}