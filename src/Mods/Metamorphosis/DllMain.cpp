#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"
#include "Cards.h"

const uint16_t METAMORPHOSIS = Cards::ARLOWNAY;
int tributedLevel = 0;
uint32_t cardDword = 0;

uint32_t __cdecl Effect_Meta(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_Meta(unsigned int* param, int param2, int param3);
uint32_t __cdecl Cost_Meta(unsigned int* param, int param2, int param3);


void Start();

bool CanBeTributed(uint8_t playerIdx, uint8_t side, uint8_t col);
void __stdcall LoadSelectionListFusion();

DWORD WINAPI MainThread(LPVOID lpParam)
{
    Sleep(500);
    Start();

    return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
        CreateThread(0, 0, &MainThread, 0, 0, NULL);
    }

    return TRUE;
}
void Start()
{
    // Uses Arlownay as base

	Register_SelectionListPopulation(METAMORPHOSIS, LoadSelectionListFusion);

    Utils::EffectScript script;
    script.CardID = METAMORPHOSIS;
    script.Effect = reinterpret_cast<uintptr_t>(&Effect_Meta);
    script.AppliesTo = 0;
    script.Condition = reinterpret_cast<uintptr_t>(&Condition_Meta);
    script.Cost = reinterpret_cast<uintptr_t>(&Cost_Meta);
    script.Target = 0;

    Register_EffectScript(script);
}
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
            FUN::InitiateSelectionList(funParam.playerIdx, 6, Cards::ARLOWNAY, 0);

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
	GameData::Player player = GameData::GetDuel().players[funParam.playerIdx];

	if (player.cardsInExtra == 0) return 0;
    if (FUN::CanPlayerSummon(funParam.playerIdx) == 0) return 0;
	if (FUN::IsCardOnField(Cards::MASK_OF_RESTRICT) != 0) return 0;

	for (size_t i = 0; i < 5; i++)
	{
        if (player.monsterZones[i].card.intID != 0)
        {
            int fieldLevel = FUN::GetMonsterLevel(player.monsterZones[i].card.intID);

			for (size_t j = 0; j < player.cardsInExtra; j++)
			{
				int extraLevel = FUN::GetMonsterLevel(player.extra[j].intID);

				if (fieldLevel == extraLevel) return 1;
			}
        }
	}

    return 0;
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

            tributedLevel = FUN::GetMonsterLevel(GameData::GetDuel().players[side].monsterZones[col].card.intID);

            GameData::SetEffectSubState(0);
             
            return 1;
        }break;
    }

    return 0;
}
bool CanBeTributed(uint8_t playerIdx,uint8_t side, uint8_t col)
{
	GameData::Player player = GameData::GetDuel().players[playerIdx];

	if (side !=  playerIdx) return false;
	if (col > 4) return false;
	if (player.monsterZones[col].card.intID == 0) return false;
	int fieldLevel = FUN::GetMonsterLevel(player.monsterZones[col].card.intID);
	for (size_t i = 0; i < player.cardsInExtra; i++)
	{
		int extraLevel = FUN::GetMonsterLevel(player.extra[i].intID);
		if (fieldLevel == extraLevel) return true;
	}
	return false;
}
void __stdcall LoadSelectionListFusion()
{
    std::vector<uint32_t> fusions;
    GameData::Player player = GameData::GetDuel().players[1];

    for (size_t i = 0; i < player.cardsInExtra; i++)
    {
		if (FUN::GetMonsterLevel(player.extra[i].intID) == tributedLevel)
		{
			fusions.push_back(player.extra[i].fullValue);
		}
    }

    GameData::ChangeSelectionList(fusions, 8);
}
