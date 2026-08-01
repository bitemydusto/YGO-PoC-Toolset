#include <Windows.h>

#include "Utils.h"
#include "GameData.h"

int isBLS = 0;
int selectionIndex = 0;

Utils::Hook hBLS_1;
Utils::Hook hBLS_2;
Utils::Hook hBLS_3;
Utils::Hook hBLS_4;
Utils::Hook hBLS_5;
Utils::Hook hBLS_6;
Utils::Hook hBLS_7;
Utils::Hook hBLS_8;
Utils::Hook hEndPhase;

void BLS();

void AddInherentSpecialSummon();
void AddSpecialSummonCondition();
void ChooseSummonState();
void ModifySelectionListPopulation();
void PatchBanishNeeded();
void RepeatSelection();
void PatchEffectToBanish();
void PatchEffectToBanish2();
void EndPhaseHook();

int CanBeSummoned();
void LoadSelectionListLight();
void LoadSelectionListDark();
void ShowDialog();
bool __cdecl AcitvationCondition(uint32_t paramAddress, int param2, int param3);
void __stdcall PostEffect(uint32_t paramAddress);
void __stdcall EndPhase();

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
	//hBLS_1 = Utils::InstallHook((void*)0x00567a43, 5, AddInherentSpecialSummon);
	hBLS_2 = Utils::InstallHook((void*)0x005aaeaf, 5, AddSpecialSummonCondition);

	hBLS_3 = Utils::InstallHook((void*)0x0059df41, 5, ChooseSummonState);
	hBLS_4 = Utils::InstallHook((void*)0x00599da4, 5, ModifySelectionListPopulation);
	hBLS_5 = Utils::InstallHook((void*)0x0059dea2, 5, PatchBanishNeeded);
	hBLS_6 = Utils::InstallHook((void*)0x0059faf6, 5, RepeatSelection);

	hBLS_7 = Utils::InstallHook((void*)0x005777d0, 8, PatchEffectToBanish);
	hBLS_8 = Utils::InstallHook((void*)0x00576890, 8, PatchEffectToBanish2);

	hEndPhase = Utils::InstallHook((void*)0x00404970, 5, EndPhaseHook);


	GameData::EffectScript script;
	script.CardID = 0x7B;
	script.Effect = 0x00585C10;
	script.AppliesTo = 0x0057A880;
	script.Condition = reinterpret_cast<uintptr_t>(&AcitvationCondition);
	script.Cost = 0x0;
	script.Target = 0x00596570;

	int idx = GameData::GetEffectScriptIndex(0x7B);
	GameData::SetEffectScript(idx, script);
}
bool __cdecl AcitvationCondition(uint32_t paramAddress, int param2, int param3)
{
	GameData::Player player = GameData::GetDuel().players[1];

	int zoneIdx = (Utils::ReadUint8((void*)(paramAddress + 0x2)) >> 1) & 0xF;
	
	if (zoneIdx < 0 || zoneIdx > 4) return false;
	if ((player.monsterZones[zoneIdx].effectIDs[31] & 0x1) == 0x1) return false;
	if (((player.monsterZones[zoneIdx].stateFlags >> 17) & 0x1) == 0x1) // Can it change position
	{
		if ((player.alreadyAttackedZones >> zoneIdx & 0x1) == 0x1) // Has it already attacked
		{
			return false;
		}
	}

	return true;
}
void __stdcall PostEffect(uint32_t paramAddress)
{
	GameData::Player player = GameData::GetDuel().players[1];
	int zoneIdx = (Utils::ReadUint8((void*)(paramAddress + 0x2)) >> 1) & 0xF;
	uint16_t stateFlag = Utils::ReadUint16((void*)(0x00a56aa8 + 0x10 + 0x90 * zoneIdx + 0x8C + 2));

	// Make it unable to attack this turn
	uint16_t mask = 0x4;
	uint16_t newFlag = stateFlag | mask;
	Utils::WriteUint16((void*)(0x00a56aa8 + 0x10 + 0x90 * zoneIdx + 0x8C + 2), newFlag);
	// Set custom once per turn flag
	Utils::WriteUint16((void*)(0x00a56aa8 + 0x10 + 0x90 * zoneIdx + 0x4A), 0x1);
}
void __stdcall EndPhase()
{
	for (size_t i = 0; i < 5; i++)
	{
		Utils::WriteUint16((void*)(0x00a56aa8 + 0x10 + 0x90 * i + 0x4A), 0x0);
	}
}
int CanBeSummoned()
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

	return (numOfLight > 0 && numOfDark > 0) ? 1 : 0;
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
__declspec(naked) void AddInherentSpecialSummon()
{
    __asm
    {
    hook:
        CMP EAX, 0x7B
        JNE hook_end
        MOV EAX, 0x00567ab0
        JMP EAX
    hook_end :
        JMP[hBLS_1.Trampoline]
    }
}
__declspec(naked) void AddSpecialSummonCondition()
{
	__asm
	{
	hook:
		CMP EAX, 0x7B
		JNE hook_end

		PUSH EAX
		CALL OFFSET CanBeSummoned
		TEST EAX, EAX
		POP EAX
		JZ hook_end

        PUSH 0x005ab148
        RET
	hook_end :
		JMP[hBLS_2.Trampoline]
	}
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
__declspec(naked) void PatchEffectToBanish()
{
	__asm
	{
	hook:
		CMP EAX, 0x7B
		JNE hook_end
		MOV isBLS, 0x1
		PUSH EAX
		PUSH EBX
		CALL OFFSET PostEffect
		POP EAX
	hook_end :
		JMP[hBLS_7.Trampoline]
	}
}
__declspec(naked) void PatchEffectToBanish2()
{
	__asm
	{
	hook:
		CMP isBLS, 0x1
		JNE hook_end
		MOV isBLS, 0x0
		MOV EAX, DWORD PTR DS:[ESP + 0x8]
		MOV ECX, DWORD PTR DS:[ESP + 0x4]
		PUSH 0x0 // No destroy sfx
		PUSH 0xf // Banish
		PUSH 0x0057689c
		RET
	hook_end :
		JMP[hBLS_8.Trampoline]
	}
}
__declspec(naked) void EndPhaseHook()
{
	__asm
	{
	hook:
		CALL OFFSET EndPhase
	hook_end:
		JMP[hEndPhase.Trampoline]
	}
}
