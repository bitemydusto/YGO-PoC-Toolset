#include "RandomCards.h"

namespace
{
	constexpr uint16_t STARDUST_DRAGON = 0x806;

	GameData::SynchroSummoner* summoner = nullptr;
	std::vector<uint32_t> tribtedInstances;
	uint32_t inst = 0;

	bool CanBeSummoned(uint32_t playerIdx)
	{
		summoner = new GameData::SynchroSummoner(playerIdx, STARDUST_DRAGON);
		bool canBeSummoned = summoner->Standard_Synchro_Condition(playerIdx);
		delete summoner;
		summoner = nullptr;

		return canBeSummoned;
	}
	uint32_t __stdcall SummonState()
	{
		if (summoner == nullptr) summoner = new GameData::SynchroSummoner(GameData::GetTurnPlayer(), STARDUST_DRAGON);
		
		uint32_t result = summoner->Standard_Synchro_SummonState();
		if (result == 1)
		{
			delete summoner;
			summoner = nullptr;
		}

		return result;
	}

	uint32_t __cdecl Effect_StardustDragon(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);
		
		if (selfParam.finishedResolving) return 0;
		if (selfParam.location == Location::GRAVE)
		{
			auto state = GameData::GetState();

			switch (state)
			{
				case 0x80:
				{
					FUN::ShowDialog("Do you want to @2Summon@0 a monster from your Graveyard to your graveyard?");
					FUN::ShowDialogOptions(1, 0);

					return 0x7f;
				}
				case 0x7f:
				{
					if (GameData::GetDialogResult() == 0)
					{
						return 0;
					}
					return 0x7e;
				}
				case 0x7e:
				{
					auto card = GameData::Card{ .fullValue = self[0] };

					int index = FUN::GetInstIndexInGrave(selfParam.playerIdx, card.GetInstance());
					if (index < 0) return 0;

					FUN::SpecialSummon(selfParam.playerIdx, &(duel->players[selfParam.playerIdx].grave[index].fullValue), 1, 1, 0x20, 0xE, selfParam.playerIdx);

					return 0;
				}
			}
		}

		if (selfParam.finishedResolving) return 0;

		FUN::W_NegateActivation(source, true);
		tribtedInstances.push_back(inst);

		return 0;
	}
	uint32_t __cdecl Condition_StardustDragon(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.location == Location::GRAVE) return 1;

		if (source == nullptr) return 0;
		if (mode != 0) return 0;

		FUN::Param sourceParam(source);
		uint16_t sourceID = FUN::GetCardID(sourceParam.cardIntID);

		if (HasTag(sourceID, CardTag::DESTROY)) return 1;


		return 0;
	}
	uint32_t __cdecl Cost_StardustDragon(unsigned int* self, unsigned int* source, int mode)
	{
		FUN::Param selfParam(self);

		if (selfParam.location == Location::GRAVE) return 1;

		auto sub = GameData::GetEffectSubState();

		if (sub == 0)
		{
			FUN::TributeSelected(selfParam.playerIdx, selfParam.zoneIdx);

			GameData::SetEffectSubState(1);
		}
		else
		{
			uint8_t n = duel->players[selfParam.playerIdx].cardsInGrave;
			inst = duel->players[selfParam.playerIdx].grave[n - 1].GetInstance();

			GameData::SetEffectSubState(0);
			return 1;
		}


		return 0;
	}
	void __stdcall EndPhase()
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < duel->players[i].cardsInGrave; j++)
			{
				auto& card = duel->players[i].grave[j];
				if (card.GetCardID() == STARDUST_DRAGON)
				{
					for (auto item : tribtedInstances)
					{
						auto inst = card.GetInstance();
						if (item == inst)
						{
							//uint32_t pack = ((uint32_t)(0xe & 0x1F) | ((uint32_t)i << 0xf) | 0x0A20u) << 16 | card.GetIntID();

							//FUN::QueueEffect(pack, card.GetInstance(), 0);

							FUN::SpecialSummon(i, &(duel->players[i].grave[j].fullValue), 1, 0x20, 0xE, i);
						}
					}
				}
			}
		}
		tribtedInstances.clear();
	}
}

void Install_StardustDragon()
{
	Register_SynchroMonster(STARDUST_DRAGON);
	Register_ActivatableEffect(STARDUST_DRAGON);
	Register_SpellSpeed(STARDUST_DRAGON, 2);
	Register_ExtraSummonMonster(STARDUST_DRAGON, CanBeSummoned, SummonState);
	Register_Phase(5, EndPhase);

	Register_EffectScript({
		.CardID = STARDUST_DRAGON,
		.Effect = (uintptr_t)Effect_StardustDragon,
		.AppliesTo = 0,
		.Condition = (uintptr_t)Condition_StardustDragon,
		.Cost = (uintptr_t)Cost_StardustDragon,
		.Target = 0
		});
}