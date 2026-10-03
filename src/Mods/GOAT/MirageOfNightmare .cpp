#include "GOAT.h"

namespace
{
	constexpr uint16_t MIRAGE_OF_NIGHTMARE = Cards::THE_DRDEK;

	uint32_t __cdecl Effect_Mirage(unsigned int* self, unsigned int* source, int mode);

	void __stdcall StandbyPhase();

	uint32_t __cdecl Effect_Mirage(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (GameData::GetPhase() > 1 || selfParam.zoneIdx > 9) return 0;

		uint16_t cardIntID = duel->players[selfParam.playerIdx].cardZones[selfParam.zoneIdx].card.GetIntID();
		uint16_t cardID = FUN::GetCardID(cardIntID);

		if (cardID != MIRAGE_OF_NIGHTMARE) return 0;

		if (GameData::GetTurnPlayer() == selfParam.playerIdx)
		{
			for (size_t i = 0; i < duel->players[selfParam.playerIdx].cardZones[selfParam.zoneIdx].effectCount; i++)
			{
				uint16_t effectIntID = duel->players[selfParam.playerIdx].cardZones[selfParam.zoneIdx].effectIDs[i];
				uint16_t effectID = FUN::GetCardID(effectIntID);
				if (effectID == MIRAGE_OF_NIGHTMARE)
				{
					int n = duel->players[selfParam.playerIdx].cardZones[selfParam.zoneIdx].effectEntries[i].value;

					if (n != 0)
					{
						if (duel->players[selfParam.playerIdx].cardsInHand < n)
						{
							n = duel->players[selfParam.playerIdx].cardsInHand;
						}
						FUN::DiscardRandomCard(selfParam.playerIdx, 0, n);
						FUN::RemoveEffectEntity(selfParam.playerIdx, selfParam.location, i);

						return 0;

					}

				}
			}
		}
		else
		{
			int n = 4 - duel->players[selfParam.playerIdx].cardsInHand;

			FUN::W_AddEffectEntityToZone(selfParam.playerIdx, selfParam.zoneIdx, selfParam.cardIntID, (n << 8) | 0xB);
			FUN::DrawCards(selfParam.playerIdx, n);
		}
		return 0;
	}
	void __stdcall StandbyPhase()
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 5; j < 10; j++)
			{
				uint16_t cardIntID = duel->players[i].cardZones[j].card.GetIntID();
				uint16_t cardID = FUN::GetCardID(cardIntID);
				if (cardID == MIRAGE_OF_NIGHTMARE)
				{
					if (GameData::GetTurnPlayer() != i && duel->players[i].cardsInHand > 4) return;

					uint32_t pack = ((uint32_t)(j & 0x1F) | ((uint32_t)i << 0xf) | 0x0A20u) << 16 | cardIntID;
					uint32_t inst = duel->players[i].cardZones[j].card.GetInstance();

					FUN::QueueEffect(pack, inst, 0);
				}

			}
		}
	}
}

void Install_Mirage()
{
	Register_Phase(1, StandbyPhase);
	
	Register_EffectScript({
		.CardID = MIRAGE_OF_NIGHTMARE,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_Mirage),
		.AppliesTo = 0,
		.Condition = 0,
		.Cost = 0,
		.Target = 0
		});
}