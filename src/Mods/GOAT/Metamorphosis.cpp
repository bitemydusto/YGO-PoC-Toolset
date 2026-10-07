#include "GOAT.h"

namespace
{
    constexpr uint16_t METAMORPHOSIS = Cards::ARLOWNAY;

    int tributedLevel = 0;
    std::vector<int> validLevels;
    uint32_t cardDword = 0;

    uint32_t __cdecl Effect_Meta(unsigned int* param, int param2, int param3);
    uint32_t __cdecl Condition_Meta(unsigned int* param, int param2, int param3);
    uint32_t __cdecl Cost_Meta(unsigned int* param, int param2, int param3);

    bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col);
    void __stdcall LoadSelectionListFusion(uint32_t playerIdx);

    uint32_t __cdecl Effect_Meta(unsigned int* param, int param2, int param3)
    {
        FUN::Param funParam(param);

        if (funParam.finishedResolving) return 0;

        uint8_t state = GameData::GetEffectState();

        switch (state)
        {
        case 0x80:
        {
            FUN::ShowDialog("Select a @3Fusion Monster@0 to summon.");

            return 0x7f;
        }
        case 0x7f:
        {
            FUN::InitiateSelectionList(funParam.playerIdx, 6, Cards::ARLOWNAY, Location::EXTRA);

            return 0xfe;
        }
        case 0xfe:
        {
            uint32_t count = FUN::GetSelectionListCount();
            if (count == 0) return 0;

            uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
            if (!entry || (*entry & 0xFFF) == 0) return 0xfe; // not ready

            cardDword = *entry;

            return 0xfd;
        }
        case 0xfd:
        {
            FUN::SpecialSummon(funParam.playerIdx, &cardDword, 1, 0x20, 0x0C, 0);

            return 0;
        }
        }
    }
    uint32_t __cdecl Condition_Meta(unsigned int* param, int param2, int param3)
    {
        FUN::Param funParam(param);
        GameData::Player player = duel->players[funParam.playerIdx];

        if (player.cardsInExtra == 0) return 0;
        if (FUN::CanPlayerSummon(funParam.playerIdx) == 0) return 0;
        if (FUN::IsCardOnField(Cards::MASK_OF_RESTRICT) != 0) return 0;

        validLevels.clear();
        for (size_t i = 0; i < 5; i++)
        {
            if (player.cardZones[i].card.GetIntID() != 0)
            {
                int fieldLevel = FUN::GetMonsterLevel(player.cardZones[i].card.GetIntID());

                for (size_t j = 0; j < player.cardsInExtra; j++)
                {
					if (IsSynchroMonster(player.extra[j].GetCardID())) continue;
                    int extraLevel = FUN::GetMonsterLevel(player.extra[j].GetIntID());

                    if (fieldLevel == extraLevel && FUN::CanBeSummonedByEffect(funParam.playerIdx, player.extra[j].GetIntID()))
                    {
                        validLevels.push_back(fieldLevel);
                    }
                }
            }
        }

        return validLevels.size() == 0 ? 0 : 1;
    }
    uint32_t __cdecl Cost_Meta(unsigned int* param, int param2, int param3)
    {
        FUN::Param funParam(param);

        uint8_t sub = GameData::GetEffectSubState();

        switch (sub)
        {
        case 0:
        {
            FUN::ShowDialog("Select a monster to Tribute.");
            GameData::SetEffectSubState(1);
        }break;
        case 1:
        {
            if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

            uint8_t side = GameData::GetSelectedSide();
            uint8_t col = GameData::GetSelectedColumn();

            if (!CanBeTributed(funParam.playerIdx, side, col)) return 0;

            if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

            FUN::MarkZoneAsTributed(side, col);

            FUN::TributeSelected(side, col);

            tributedLevel = FUN::GetMonsterLevel(duel->players[side].cardZones[col].card.GetIntID());

            GameData::SetEffectSubState(0);

            return 1;
        }break;
        }

        return 0;
    }
    bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col)
    {
        GameData::Player player = duel->players[playerIdx];

        if (side != playerIdx) return false;
        if (col > 4) return false;
        if (player.cardZones[col].card.GetIntID() == 0) return false;

        int fieldLevel = FUN::GetMonsterLevel(player.cardZones[col].card.GetIntID());
        for (const auto& level : validLevels)
        {
            if (fieldLevel == level) return true;
        }
        return false;
    }
    void __stdcall LoadSelectionListFusion(uint32_t playerIdx)
    {
        std::vector<uint32_t> fusions;
        GameData::Player player = duel->players[GameData::GetTurnPlayer()];

        for (size_t i = 0; i < player.cardsInExtra; i++)
        {
			if (IsSynchroMonster(player.extra[i].GetCardID())) continue;
            if (FUN::GetMonsterLevel(player.extra[i].GetIntID()) == tributedLevel && FUN::CanBeSummonedByEffect(GameData::GetTurnPlayer(), player.extra[i].GetIntID()))
            {
                fusions.push_back(player.extra[i].fullValue);
            }
        }

        GameData::ChangeSelectionList(fusions, 8);
    }
}

void Install_Metamorphosis()
{
	Register_SelectionListPopulation(METAMORPHOSIS, LoadSelectionListFusion);

	Register_EffectScript({
		.CardID = METAMORPHOSIS,
		.Effect = reinterpret_cast<uintptr_t>(&Effect_Meta),
		.AppliesTo = 0,
		.Condition = reinterpret_cast<uintptr_t>(&Condition_Meta),
		.Cost = reinterpret_cast<uintptr_t>(&Cost_Meta),
		.Target = 0
		});
}