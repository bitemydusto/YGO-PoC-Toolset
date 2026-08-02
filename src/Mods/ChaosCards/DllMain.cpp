#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"

int isBLS = 0;
int selectionIndex = 0;

Utils::Hook hBLS_1;
Utils::Hook hBLS_2;
Utils::Hook hBLS_3;
Utils::Hook hBLS_4;
Utils::Hook hBLS_5;
Utils::Hook hBLS_6;
Utils::Hook hEndPhase;

void BLS();

void ChooseSummonState();
void ModifySelectionListPopulation();
void PatchBanishNeeded();
void RepeatSelection();
void EndPhaseHook();

bool CanBeSummoned();
void LoadSelectionListLight();
void LoadSelectionListDark();
void ShowDialog();

uint32_t __cdecl Effect_BLS(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_BLS(uint32_t paramAddress, int param2, int param3);
uint32_t __cdecl Cost_BLS(uint32_t paramAddress, int param2, int param3);

void __stdcall EndPhase(uint32_t phase);

DWORD WINAPI MainThread(LPVOID lpParam)
{
	Sleep(500);
    BLS();

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
void BLS()
{
	Register_SpecialSummonCondition(0x7B, CanBeSummoned);

	hBLS_3 = Utils::InstallHook((void*)0x0059df41, 5, ChooseSummonState);
	hBLS_4 = Utils::InstallHook((void*)0x00599da4, 5, ModifySelectionListPopulation);
	hBLS_5 = Utils::InstallHook((void*)0x0059dea2, 5, PatchBanishNeeded);
	hBLS_6 = Utils::InstallHook((void*)0x0059faf6, 5, RepeatSelection);

	hEndPhase = Utils::InstallHook((void*)0x00404970, 5, EndPhaseHook);


	GameData::EffectScript script;
	script.CardID = 0x7B;
	script.Effect = reinterpret_cast<uintptr_t>(&Effect_BLS);
	script.AppliesTo = 0x0057A880;
	script.Condition = reinterpret_cast<uintptr_t>(&Condition_BLS);
	script.Cost = reinterpret_cast<uintptr_t>(&Cost_BLS);
	script.Target = 0x00596570;

	int idx = GameData::GetEffectScriptIndex(0x7B);
	GameData::SetEffectScript(idx, script);
}
uint32_t __cdecl Effect_BLS(unsigned int* param, int param2, int param3)
{
	uint8_t* block = (uint8_t*)param;

	// Has effect finished resolving?
	if (block[4] & 4) return 0;

	// Get target parameters
	uint32_t target = 0;
	if (!FUN::SetTargetParams((uint32_t)block, 0, &target)) return 0;

	uint32_t side = target & 0xff;          // as in original (player in low bits)
	uint32_t zone = (target >> 8) & 0xff;

	uint32_t* zoneCard = (uint32_t*)(0x00a55d74 + (side & 1) * 0xD44 + zone * 0x90);
	if ((*zoneCard & 0xfff) == 0) return 0;

	// Banish: dest 0xF, flags 0 (like FUN_005778a0)
	uint32_t mask = 1u << ((((int)(char)side) << 4) + (char)zone & 0x1f);
	FUN::SendCardFromField(block, mask, 0xf, 0);

	return 0;
}
uint32_t __cdecl Condition_BLS(uint32_t paramAddress, int param2, int param3)
{
	//uint8_t playerIdx = *(param + 2) & 0x1;
	//uint8_t zoneIdx = (*(param + 2) >> 1) & 0x1;
	uint8_t zoneIdx = (Utils::ReadUint8((void*)(paramAddress + 0x2)) >> 1) & 0x1;
	uint8_t playerIdx = Utils::ReadUint8((void*)(paramAddress + 0x2)) & 0x1;
	GameData::Player player = GameData::GetDuel().players[playerIdx];

	
	if (zoneIdx < 0 || zoneIdx > 4) return 0;
	// Used its effect this turn
	if ((player.monsterZones[zoneIdx].effectIDs[31] & 0x1) == 0x1) return 0;
	// Can't change position
	if ((player.monsterZones[zoneIdx].stateFlags & 0x20000) != 0)
	{
		// Attacked this turn
		if (((player.alreadyAttackedZones >> zoneIdx) & 0x1) == 0x1) return 0;
	}

	return 1;
}
uint32_t __cdecl Cost_BLS(uint32_t paramAddress, int param2, int param3)
{
	//uint8_t playerIdx = *(param + 2) & 0x1;
	//uint8_t zoneIdx = (*(param + 2) >> 1) & 0x1;
	uint8_t zoneIdx = (Utils::ReadUint8((void*)(paramAddress + 0x2)) >> 1) & 0x1;
	uint8_t playerIdx = Utils::ReadUint8((void*)(paramAddress + 0x2)) & 0x1;

	GameData::Player player = GameData::GetDuel().players[playerIdx];

	// Make it unable to attack this turn
	uint16_t stateFlag = player.monsterZones[zoneIdx].stateFlags | 0x4;
	Utils::WriteUint16((void*)(0x00a55d64 + playerIdx * 0xD44 + 0x10 + 0x90 * zoneIdx + 0x8C + 2), stateFlag);
	// Set custom once per turn flag
	Utils::WriteUint16((void*)(0x00a55d64 + playerIdx * 0xD44 + 0x10 + 0x90 * zoneIdx + 0x4A), 0x1);

	return 1;
}
void __stdcall EndPhase(uint32_t phase)
{
	if (phase != 5) return;
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			Utils::WriteUint16((void*)(0x00a55d64 + i * 0xD44 + 0x10 + j* 0x90 + 0x4A), 0x0);
		}
	}
}
bool CanBeSummoned()
{
	GameData::Player player = GameData::GetDuel().players[1];

	int numOfLight = 0;
	int numOfDark = 0;
	for (size_t i = 0; i < player.cardsInGrave; i++)
	{
		if (player.grave[i].GetType() < 0x15)
		{
			uint32_t attr = player.grave[i].GetAttribute();
			if (attr == 0x1) // Light
			{
				numOfLight++;
			}
			else if (attr == 0x2) // Dark
			{
				numOfDark++;
			}
		}
	}

	return (numOfLight > 0 && numOfDark > 0) ? true : false;
}
void ShowDialog()
{
	FUN::ShowDialogue("Select @31@0 @2LIGHT@0 and @31@0 @2DARK@0 monster from your Graveyard to banish.");
}
void LoadSelectionListLight()
{
	std::vector<uint32_t> lightCards;

	GameData::Player player = GameData::GetDuel().players[1];
	for (size_t i = 0; i < player.cardsInGrave; i++)
	{
		if (player.grave[i].GetType() < 0x15)
		{
			uint32_t attr = player.grave[i].GetAttribute();
			if (attr == 0x1) // Light
			{
				lightCards.push_back(player.grave[i].fullValue);
			}
		}
	}

	GameData::ChangeSelectionList(lightCards);
}
void LoadSelectionListDark()
{
	std::vector<uint32_t> darkCards;

	GameData::Player player = GameData::GetDuel().players[1];
	for (size_t i = 0; i < player.cardsInGrave; i++)
	{
		if (player.grave[i].GetType() < 0x15)
		{
			uint32_t attr = player.grave[i].GetAttribute();
			if (attr == 0x2) // Dark
			{
				darkCards.push_back(player.grave[i].fullValue);
			}
		}
	}

	GameData::ChangeSelectionList(darkCards);
}
__declspec(naked) void ChooseSummonState()
{
	__asm
	{
	hook:
		CMP EAX, 0x7B
		JNE hook_end

		CALL OFFSET ShowDialog

		MOV CL, BYTE PTR DS:[0x00a57808]
		POP EDI
		AND ECX, 0xFF
		POP ESI
		OR CH, 0x33 // Selection state
		POP EBP
		MOV WORD PTR DS:[0x00a57808], CX

		PUSH 0x0059e0e1
		RET
	hook_end :
		JMP[hBLS_3.Trampoline]
	}
}
__declspec(naked) void ModifySelectionListPopulation()
{
	__asm
	{
	hook:
		CMP BX, 0x7B
		JNE hook_end
		MOV ESI, 0x4	
		MOV DWORD PTR DS:[ESP + 0x10], 0x1
		CMP selectionIndex, 0x0
		JNE hook_dark
	hook_light:
		PUSH EAX
		CALL OFFSET LoadSelectionListLight
		POP EAX
		PUSH 0x0059c432
		RET
	hook_dark:
		PUSH EAX
		CALL OFFSET LoadSelectionListDark
		POP EAX
		PUSH 0x0059c432
		RET
	hook_end :
		CMP EBX, 0x460
		JMP[hBLS_4.Trampoline]
	}
}
__declspec(naked) void PatchBanishNeeded()
{
	__asm
	{
	hook:
		CMP EAX, 0x7B
		JNE hook_end
		PUSH 0x0059debf
		RET
	hook_end :
		JMP[hBLS_5.Trampoline]
	}
}
__declspec(naked) void RepeatSelection()
{
	__asm
	{
	hook:

		CMP WORD PTR DS:[0x00a57802], 0x5 // Check intID of card used
		JNE hook_end

		CMP selectionIndex, 0x0
		JNE hook_no_repeat

	hook_repeat:
		MOV selectionIndex, 0x1
		MOV EAX, DWORD PTR DS:[0x00a57808] // Load state
		AND EAX, 0x00FF
		OR EAX, 0x3200
		MOV WORD PTR DS:[0x00a57808], AX // Set state to 0x32 so it will increaced to 0x33 and repeat the selection
		MOV BYTE PTR DS:[0x00A5780A], 0 // Reset selected so far
		JMP hook_end
	hook_no_repeat:
		MOV selectionIndex, 0x0
		PUSH EAX
		MOV EAX, DWORD PTR DS:[0x00a55080]
		AND EAX, 0xf1ffffff
		MOV DWORD PTR DS:[0x00a55080], EAX
		POP EAX
	hook_end :
		JMP[hBLS_6.Trampoline]
	}
}
__declspec(naked) void EndPhaseHook()
{
	__asm
	{
	hook:
        PUSH DWORD PTR DS:[ESP+4]
		CALL OFFSET EndPhase
	hook_end:
		JMP[hEndPhase.Trampoline]
	}
}
