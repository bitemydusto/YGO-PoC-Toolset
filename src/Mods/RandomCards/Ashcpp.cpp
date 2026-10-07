#include "RandomCards.h"

namespace
{
	constexpr uint16_t ASH = 0x808;

	uint32_t __cdecl Effect_ASH(EffectBlock* self, EffectBlock* source, int mode)
	{
		if (self->GetFinishedResolving()) return 0;
		if (source == nullptr) return 0;

		FUN::W_NegateActivation((unsigned int*)source, false);

		return 0;
	}
	uint32_t __cdecl Condition_ASH(EffectBlock* self, EffectBlock* source, int mode)
	{
		if (source == nullptr) return 0;
		if (CardWasUsedThisTurn(ASH, self->GetSide())) return 0;
		uint16_t cardID = FUN::GetCardID(source->GetIntID());
		if (HasTag(cardID, CardTag::DRAW) ||
			HasTag(cardID, CardTag::SEARCH) ||
			HasTag(cardID, CardTag::SEND_FROM_DECK) ||
			HasTag(cardID, CardTag::SUMMON_DECK))
		{
			return 1;
		}

		return 0;
	}
	uint32_t __cdecl Cost_ASH(EffectBlock* self, EffectBlock* source, int mode)
	{
		uint8_t handIdx = FUN::GetInstIndexInHand(self->GetSide(), self->GetInstance());

		if (handIdx > -1) FUN::DiscardFromHand(self->GetSide(), handIdx, 0);

		SetHardOncePerTurnFlag(ASH, self->GetSide());

		return 1;
	}

}

void Install_Ash()
{
	Register_TunerMonster(ASH);
	Register_HasEffectInHand(ASH);
	Register_ResponseWindow(ASH, 0x3f);
	Register_SpellSpeed(ASH, 2);

	Register_EffectScript({
		.CardID = ASH,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_ASH),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(&Condition_ASH),
		.Cost = reinterpret_cast<uintptr_t>(&Cost_ASH),
		.Target = 0
		});
}