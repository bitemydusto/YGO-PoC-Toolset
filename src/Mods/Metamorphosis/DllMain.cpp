#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"

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
	Register_SelectionListPopulation(0xB3, LoadSelectionListFusion);

    Utils::EffectScript script;
    script.CardID = 0xB3;
    script.Effect = reinterpret_cast<uintptr_t>(&Effect_Meta);
    script.AppliesTo = 0;
    script.Condition = reinterpret_cast<uintptr_t>(&Condition_Meta);
    script.Cost = reinterpret_cast<uintptr_t>(&Cost_Meta);
    script.Target = 0;

    ReplaceEffectScript(0x523, script);
}
uint32_t __cdecl Effect_Meta(unsigned int* param, int param2, int param3)
{
    uint8_t* block = (uint8_t*)param;
    uint8_t  playerIdx = block[2] & 1;

    uint8_t state = Utils::ReadUint8((void*)(0x00a55c88 + 2));

    switch (state)
    {
        case 0x80:
        {
            // Clear target count bits
            *(uint16_t*)(block + 4) &= 0x1FFF;
            FUN::ShowDialog("Select a @3Fusion Monster@0 to summon.");

            return 0x7f;
        }
        case 0x7f:
        {
            FUN::InitiateSelectionList(playerIdx, 6, 0xB3, 1);

            return 0xfe;
        }
        case 0xfe:
        {
            uint32_t count = FUN::GetSelectionListCount();
            if (count == 0)
            {
                Utils::WriteUint8((void*)0x00A55C8E, 0);
                return 0;
            }

            uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
            if (!entry || (*entry & 0xFFF) == 0) return 0xfe; // not ready

			cardDword = *entry;

            return 0xfd;
        }
		case 0xfd:
		{
            FUN::SpecialSummon(playerIdx, &cardDword, 1, 0x20, 0x0C, 0);

            return 0;
		}
    }
}
uint32_t __cdecl Condition_Meta(unsigned int* param, int param2, int param3)
{
    uint8_t* block = (uint8_t*)param;
    uint8_t  playerIdx = block[2] & 1;
	GameData::Player player = GameData::GetDuel().players[playerIdx];

	if (player.cardsInExtra == 0) return 0;
    if (FUN::CanPlayerSummon(playerIdx) == 0) return 0;
	if (FUN::IsCardOnField(0x58A) != 0) return 0;

	for (size_t i = 0; i < 5; i++)
	{
        if (player.monsterZones[i].card.intID != 0)
        {
			for (size_t j = 0; j < player.cardsInExtra; j++)
			{
				int fieldLevel = FUN::GetMonsterLevel(player.monsterZones[i].card.intID);
				int extraLevel = FUN::GetMonsterLevel(player.extra[j].intID);

				if (fieldLevel == extraLevel) return 1;
			}
        }
	}

    return 0;
}
uint32_t __cdecl Cost_Meta(unsigned int* param, int param2, int param3)
{
    uint8_t* block = (uint8_t*)param;
    uint8_t  playerIdx = block[2] & 1;

    uint8_t sub = Utils::ReadUint8((void*)0x00A55C8E);

    switch (sub)
    {
        case 0:
        {
            FUN::ShowDialog("Select a monster to Tribute.");
            Utils::WriteUint8((void*)0x00A55C8E, 1);
        }break;
        case 1:
        {
            if (FUN::IsFieldSelectionReady(0xf000f0) == 0) return 0;

			uint8_t side = Utils::ReadUint8((void*)0x000a55044);
			uint8_t col = Utils::ReadUint8((void*)0x00a5504c);

			if (!CanBeTributed(playerIdx, side, col)) return 0;

			if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

			FUN::MarkZoneAsTributed(side, col);

			FUN::TributeSelected(side, col);

            tributedLevel = FUN::GetMonsterLevel(GameData::GetDuel().players[side].monsterZones[col].card.intID);

            Utils::WriteUint8((void*)0x00A55C8E, 0);
             
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
