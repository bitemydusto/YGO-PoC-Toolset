#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"

using Original_t = uint32_t(__cdecl*)(unsigned int* param, unsigned int param2, unsigned int param3);
inline Original_t Original = reinterpret_cast<Original_t>(0x00584200);
using OriginalTarget_t = uint32_t(__cdecl*)(unsigned int* param, int param2, int param3);
inline OriginalTarget_t OriginalTarget = reinterpret_cast<OriginalTarget_t>(0x00593520);

int isBLS = 0;
int selectionIndex = 0;
unsigned int cedDamage;

Utils::Hook hBLS_3;
Utils::Hook hBLS_4;
Utils::Hook hBLS_5;
Utils::Hook hBLS_6;

Utils::Hook hDMOC_1;



void Chaos();

void ChooseSummonState();
void ModifySelectionListPopulation();
void PatchBanishNeeded();
void RepeatSelection();

bool CanBeSummoned();
void LoadSelectionListLight();
void LoadSelectionListDark();
void ShowDialog();

uint32_t __cdecl Effect_BLS(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_BLS(uint32_t paramAddress, int param2, int param3);
uint32_t __cdecl Cost_BLS(uint32_t paramAddress, int param2, int param3);

uint32_t __cdecl Effect_CED(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_CED(uint32_t paramAddress, int param2, int param3);
uint32_t __cdecl Cost_CED(uint32_t paramAddress, int param2, int param3);

uint32_t __cdecl Effect_DMOC(unsigned int* param, int param2, int param3);
uint32_t __cdecl Target_DMOC(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_DMOC(uint32_t paramAddress, int param2, int param3);


void __stdcall EndPhase();
void __stdcall BLS_DoubleAttack();
void __stdcall DMOC_BanishOnKill();

DWORD WINAPI MainThread(LPVOID lpParam)
{
	Sleep(500);
    Chaos();

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
void Chaos()
{
	Register_SpecialSummonCondition(0x7B, CanBeSummoned);
	Register_SpecialSummonCondition(0x1BD, CanBeSummoned);
	Register_Phase(5, EndPhase);
	Register_AfterDamageCalculation(BLS_DoubleAttack);
	Register_AfterDamageCalculation(DMOC_BanishOnKill);
	Register_NormalSummonTrigger(0x10A);
	Register_SpecialSummonTrigger(0x10A);
	Register_BanishOnLeavingField(0x10A);

	hBLS_3 = Utils::InstallHook((void*)0x0059df41, 5, ChooseSummonState);
	hBLS_4 = Utils::InstallHook((void*)0x00599da4, 5, ModifySelectionListPopulation);
	hBLS_5 = Utils::InstallHook((void*)0x0059dea2, 5, PatchBanishNeeded);
	hBLS_6 = Utils::InstallHook((void*)0x0059faf6, 5, RepeatSelection);


	GameData::EffectScript scriptBLS;
	scriptBLS.CardID = 0x7B;
	scriptBLS.Effect = reinterpret_cast<uintptr_t>(&Effect_BLS);
	scriptBLS.AppliesTo = 0x0057A880;
	scriptBLS.Condition = reinterpret_cast<uintptr_t>(&Condition_BLS);
	scriptBLS.Cost = reinterpret_cast<uintptr_t>(&Cost_BLS);
	scriptBLS.Target = 0x00596570;

	int idx = GameData::GetEffectScriptIndex(0x7B);
	GameData::SetEffectScript(idx, scriptBLS);


	GameData::EffectScript scriptCED;
	scriptCED.CardID = 0x1BD;
	scriptCED.Effect = reinterpret_cast<uintptr_t>(&Effect_CED);
	scriptCED.AppliesTo = 0;
	scriptCED.Condition = reinterpret_cast<uintptr_t>(&Condition_CED);
	scriptCED.Cost = reinterpret_cast<uintptr_t>(&Cost_CED);
	scriptCED.Target = 0;

	idx = GameData::GetEffectScriptIndex(0x1BD);
	GameData::SetEffectScript(idx, scriptCED);

	GameData::EffectScript scriptDMOC;
	scriptDMOC.CardID = 0x10A;
	scriptDMOC.Effect = reinterpret_cast<uintptr_t>(&Effect_DMOC);
	scriptDMOC.AppliesTo = 0;
	scriptDMOC.Condition = reinterpret_cast<uintptr_t>(&Condition_DMOC);
	scriptDMOC.Cost = 0;
	scriptDMOC.Target = reinterpret_cast<uintptr_t>(&Target_DMOC);

	idx = GameData::GetEffectScriptIndex(0x10A);
	GameData::SetEffectScript(idx, scriptDMOC);
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
uint32_t __cdecl Effect_DMOC(unsigned int* param, int param2, int param3)
{
	uint8_t* block = (uint8_t*)param;
	// Has effect finished resolving?
	if (block[4] & 4) return 0;

	//uint16_t loc = (block[2] >> 1) & 0x1F;
	uint8_t playerIdx = block[2] & 0x1;
	GameData::Player player = GameData::GetDuel().players[playerIdx];


	uint16_t lo = *(uint16_t*)(block + 6);
	uint16_t hi = *(uint16_t*)(block + 8);
	uint32_t cardDword = lo | ((uint32_t)hi << 16);

	uint16_t intId = cardDword & 0xFFF;
	uint8_t  owner = (cardDword >> 12) & 1;
	uint32_t inst = owner + ((cardDword >> 24) & 0x7F) * 2;

	if (intId == 0) return 0;

	FUN::AddTargetedCardToHand(block, playerIdx, &cardDword);

	return 0;
}
uint32_t __cdecl Condition_DMOC(uint32_t paramAddress, int param2, int param3)
{
	uint8_t playerIdx = Utils::ReadUint8((void*)(paramAddress + 0x2)) & 0x1;
	GameData::Player player = GameData::GetDuel().players[playerIdx];
	int spells = 0;
	for (size_t i = 0; i < player.cardsInGrave; i++)
	{
		if (player.grave[i].GetType() == 0x16) spells++;
	}
	return spells > 0 ? 1 : 0;
}
uint32_t __cdecl Target_DMOC(unsigned int* param, int param2, int param3)
{
	uint8_t* block = (uint8_t*)param;
	uint8_t  playerIdx = block[2] & 1;
	//uint8_t  loc = (block[2] >> 1) & 0x1F;

	uint8_t sub = Utils::ReadUint8((void*)0x00A55C8E);

	switch (sub)
	{
	case 0:
	{
		FUN::ShowDialog("Do you want to add a @2Spell Card@0 from your Graveyard to your hand?");
		FUN::ShowDialogOptions(1, 0);

		Utils::WriteUint8((void*)0x00A55C8E, 1);
		return 0;
	}
	case 1:
	{
		if (Utils::ReadUint8((void*)0x00a558b4) != 0)
		{
			Utils::WriteUint8((void*)0x00A55C8E, 2);
			return 0;
		}
		return 1;
	}
	case 2:
	{
		// Clear target count bits
		*(uint16_t*)(block + 4) &= 0x1FFF;

		FUN::ShowDialog("Select a @2Spell Card@0 from your Graveyard to add to your hand.");

		Utils::WriteUint8((void*)0x00A55C8E, 3);
		return 0;
	}
	case 3:
	{
		FUN::InitiateSelectionList(playerIdx, 6, 0x1AB, 0);

		Utils::WriteUint8((void*)0x00A55C8E, 4);
		return 0;
	}

	case 4:
	{
		uint32_t count = FUN::GetSelectionListCount();
		if (count == 0)
		{
			Utils::WriteUint8((void*)0x00A55C8E, 0);
			return 1;
		}

		uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
		if (!entry || (*entry & 0xFFF) == 0)
			return 0; // not ready

		uint32_t dword = *entry;
		uint8_t  owner = (dword >> 12) & 1;
		uint32_t inst = owner + ((dword >> 24) & 0x7F) * 2;
		uint32_t sideBit = owner ? 0x8000u : 0;

		uint32_t cardId = FUN::GetCardID(dword & 0xFFF); // match your GetCardID arity

		// Highlight / reveal
		FUN::HighLightCard(sideBit | 0xDF, cardId, 0, 0);
		FUN::HighLightCard(sideBit | 0x08, owner, 0x0E, 0);  // 0x0E = GY

		// Store targets
		FUN::FUN_00592a40((int)param, (uint16_t)dword);
		FUN::FUN_00592a40((int)param, (uint16_t)(dword >> 16));

		Utils::WriteUint8((void*)0x00A55C8E, 0);
		return 1;
	}

	default:
		Utils::WriteUint8((void*)0x00A55C8E, 0);
		return 1;
	}
}
uint32_t __cdecl Effect_CED(unsigned int* param, int param2, int param3)
{
	uint8_t* block = (uint8_t*)param;

	// Has effect finished resolving?
	if (block[4] & 4) return 0;

	uint8_t state = Utils::ReadUint8((void*)(0x00a55c88 + 2));
	uint8_t playerIdx = block[2] & 0x1;
	uint8_t opp = playerIdx ^ 0x1;
	GameData::Duel duel = GameData::GetDuel();


	switch (state)
	{
		case 0x80:
		{
			unsigned int cardsOnPlayerField = 0;
			unsigned int cardsOnOppField = 0;
			for (size_t i = 0; i < 5; i++)
			{
				if (duel.players[opp].monsterZones[i].card.intID != 0) cardsOnOppField++;
				if (duel.players[opp].spellTrapZones[i].card.intID != 0) cardsOnOppField++;
				if (duel.players[playerIdx].monsterZones[i].card.intID != 0) cardsOnPlayerField++;
				if (duel.players[playerIdx].spellTrapZones[i].card.intID != 0) cardsOnPlayerField++;
			}
			if (duel.players[opp].fieldSpell.intID != 0) cardsOnOppField++;
			if (duel.players[playerIdx].fieldSpell.intID != 0) cardsOnPlayerField++;
			cedDamage = (duel.players[opp].cardsInHand + cardsOnOppField + duel.players[playerIdx].cardsInHand + cardsOnPlayerField) * 300;

		}break;
		case 0x7f:
		{
			if (duel.players[1].cardsInHand != 0)
			{
				FUN::DiscardFromHand(1, 0, 1);
				return 0x7f;
			}
			return 0x7e;
		}break;
		case 0x7e:
		{
			if (duel.players[0].cardsInHand != 0)
			{
				FUN::DiscardFromHand(0, 0, 1);
				return 0x7e;
			}
			return 0x7d;
		}break;
		case 0x7d:
		{
			FUN::SendCardFromField(block, 0x07ff07ff, 0xe, 0);
			return 0x7c;
		}break;
		case 0x7c:
		{
			FUN::DealEffectDamage(!playerIdx, cedDamage);
			return 0;
		}break;
	}


	return 0x7f;
}
uint32_t __cdecl Condition_CED(uint32_t paramAddress, int param2, int param3)
{
	uint8_t playerIdx = Utils::ReadUint8((void*)(paramAddress + 0x2)) & 0x1;
	GameData::Player player = GameData::GetDuel().players[playerIdx];

	if (player.lifePoints <= 1000) return 0;

	return 1;
}
uint32_t __cdecl Cost_CED(uint32_t paramAddress, int param2, int param3)
{
	uint8_t playerIdx = Utils::ReadUint8((void*)(paramAddress + 0x2)) & 0x1;
	GameData::Player player = GameData::GetDuel().players[playerIdx];
	FUN::PayLifePoints(playerIdx, 1000);
	return 1;
}
void __stdcall EndPhase()
{
	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			Utils::WriteUint16((void*)(0x00a55d64 + i * 0xD44 + 0x10 + j* 0x90 + 0x4A), 0x0);
		}
	}
}
void __stdcall BLS_DoubleAttack()
{
	GameData::BattleResult battleResult = GameData::GetBattleResult();
	uint8_t attackerIdx = battleResult.StateFlags & 0x1;
	if (battleResult.sides[attackerIdx].IntID == 0x05 && (battleResult.sides[attackerIdx ^ 0x1].ResultFlags & 0x10) != 0)
	{
		GameData::Player attacker = GameData::GetDuel().players[attackerIdx];
		uint8_t zoneIdx = (battleResult.StateFlags >> 8) & 7;
		if ((attacker.monsterZones[zoneIdx].effectIDs[31] & 0x1) == 0)
		{
			// Reset attacked flag
			uint16_t attackedFlag = attacker.alreadyAttackedZones & ~(1 << zoneIdx);
			Utils::WriteUint16((void*)(0x00a55d64 + attackerIdx * 0xD44 + 0xc), attackedFlag);
			// Set custom once per turn flag
			Utils::WriteUint16((void*)(0x00a55d64 + attackerIdx * 0xD44 + 0x10 + 0x90 * zoneIdx + 0x4A), 0x1);

		}
	}
}
void __stdcall DMOC_BanishOnKill()
{
	GameData::BattleResult battleResult = GameData::GetBattleResult();
	uint8_t attackerIdx = battleResult.StateFlags & 0x1;
	if (battleResult.sides[attackerIdx].IntID == 0x0D && (battleResult.sides[!attackerIdx].ResultFlags & 0x10) != 0)
	{
		uint8_t attackedZoneIdx = (battleResult.StateFlags >> 0xB) & 7;
		uint32_t mask = 1u << ((((int)(char)!attackerIdx) << 4) + (char)attackedZoneIdx & 0x1f);
		uint8_t block[0x20];
		memset(block, 0, sizeof(block));

		FUN::SendCardFromField(block, mask, 0xf, 0);
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
	FUN::ShowDialog("Select @31@0 @2LIGHT@0 and @31@0 @2DARK@0 monster from your Graveyard to banish.");
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
		JE hook_1
		CMP EAX, 0x1BD
		JE hook_1
		JMP hook_end

	hook_1:
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
		JE hook_1
		CMP BX, 0x1BD
		JE hook_1
		JMP hook_end

	hook_1:
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
		JE hook_1
		CMP EAX, 0x1BD
		JE hook_1
		JMP hook_end

	hook_1:
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
		JE hook_1
		CMP WORD PTR DS:[0x00a57802], 0x9
		JE hook_1
		JMP hook_end

	hook_1:
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