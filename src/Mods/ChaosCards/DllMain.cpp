#include <Windows.h>

#include "Utils.h"
#include "GameData.h"
#include "HookAPI.h"

unsigned int cedDamage;
int innerState = 0;


void Chaos();


uint32_t __cdecl Effect_BLS(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_BLS(uint32_t paramAddress, int param2, int param3);
uint32_t __cdecl Cost_BLS(uint32_t paramAddress, int param2, int param3);

uint32_t __cdecl Effect_CED(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_CED(uint32_t paramAddress, int param2, int param3);
uint32_t __cdecl Cost_CED(uint32_t paramAddress, int param2, int param3);

uint32_t __cdecl Effect_DMOC(unsigned int* param, int param2, int param3);
uint32_t __cdecl Target_DMOC(unsigned int* param, int param2, int param3);
uint32_t __cdecl Condition_DMOC(uint32_t paramAddress, int param2, int param3);


bool CanBeSummoned(uint32_t playerIdx);
void __stdcall LoadSelectionListDark();
void __stdcall EndPhase();
void __stdcall BLS_DoubleAttack();
void __stdcall DMOC_BanishOnKill();
uint32_t __stdcall SummonStates();

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
	Register_SpecialSummonCondition(0x27F, CanBeSummoned);
	Register_Phase(5, EndPhase);
	Register_AfterDamageCalculation(BLS_DoubleAttack);
	Register_AfterDamageCalculation(DMOC_BanishOnKill);
	Register_NormalSummonTrigger(0x10A);
	Register_SpecialSummonTrigger(0x10A);
	Register_BanishOnLeavingField(0x10A);
	Register_InitialSummonState(0x7B, 0x39);
	Register_InitialSummonState(0x1BD, 0x39);
	Register_InitialSummonState(0x27F, 0x39);
	Register_SummonState(0x39, SummonStates);
	Register_SelectionListPopulation(0x7B, LoadSelectionListDark);


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

	GameData::EffectScript scriptCS;
	scriptCS.CardID = 0x27F;
	scriptCS.Effect = reinterpret_cast<uintptr_t>(&Effect_BLS);
	scriptCS.AppliesTo = 0x0057AD70;
	scriptCS.Condition = reinterpret_cast<uintptr_t>(&Condition_BLS);
	scriptCS.Cost = reinterpret_cast<uintptr_t>(&Cost_BLS);
	scriptCS.Target = 0x00595250;

	idx = GameData::GetEffectScriptIndex(0x27F);
	GameData::SetEffectScript(idx, scriptCS);
}
uint32_t __cdecl Effect_BLS(unsigned int* param, int param2, int param3)
{
	uint8_t* block = (uint8_t*)param;

	// Has effect finished resolving?
	if (block[4] & 4) return 0;

	// Get target parameters
	uint32_t target = 0;
	if (!FUN::SetTargetParams((uint32_t)block, 0, &target)) return 0;

	uint32_t side = target & 0xff;
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

	if (FUN::IsCardOnSideOfField(playerIdx ^ 0x1, 0x5E7) > 0) return false;

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
		if (zoneIdx < 0 || zoneIdx > 4) return;
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

	if (FUN::IsCardOnSideOfField(!attackerIdx, 0x5E7) > 0) return;
	if (battleResult.sides[attackerIdx].IntID == 0x0D && (battleResult.sides[!attackerIdx].ResultFlags & 0x10) != 0)
	{
		uint8_t attackedZoneIdx = (battleResult.StateFlags >> 0xB) & 7;
		uint32_t mask = 1u << ((((int)(char)!attackerIdx) << 4) + (char)attackedZoneIdx & 0x1f);
		uint8_t block[0x20];
		memset(block, 0, sizeof(block));

		FUN::SendCardFromField(block, mask, 0xf, 0);
	}


}
bool CanBeSummoned(uint32_t playerIdx)
{
	if (FUN::IsCardOnSideOfField(playerIdx ^ 0x1, 0x5E7) > 0) return false;

	GameData::Player player = GameData::GetDuel().players[playerIdx];

	int numOfLight = 0;
	int numOfDark = 0;
	for (size_t i = 0; i < player.cardsInGrave; i++)
	{
		if (player.grave[i].GetType() < 0x15)
		{
			uint32_t attr = player.grave[i].GetAttribute();
			if (attr == 0x1) numOfLight++; // Light
			else if (attr == 0x2) numOfDark++; // Dark
		}
	}
	if (numOfLight > 0 && numOfDark > 0)
	{
		if (FUN::NumOfEmptyValidSummonZones(playerIdx) > 0)
		{
			if (FUN::CanPlayerSummon(playerIdx) > 0)
			{
				return true;
			}
		}
	}

	return false;
}
uint32_t __stdcall SummonStates()
{
	switch (innerState)
	{
		case 0:
		{
			FUN::ShowDialog("You must banish @31@0 @2LIGHT@0 and @31@0 @2DARK@0 monster from your Graveyard to summon this monster. Do you wish to @2Summon@0?");
			FUN::ShowDialogOptions(1, 0);
			innerState = 0xe;
		}break;
		case 0xe:
		{
			if (Utils::ReadUint8((void*)0x00a558b4) == 0)
			{
				innerState = 0;
				return 1;
			}
			innerState = 1;
		}break;
		case 1:
		{
			FUN::InitiateSelectionList(1, 6, 0x5EB, 0); // Original population logic: Soul of Purity and Light
			innerState = 2;
		}break;
		case 2:
		{
			uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
			if (!entry || (*entry & 0xFFF) == 0) return 0; // not ready

			uint32_t dword = *entry;
			FUN::BanishCardFromGrave(1, &dword);

			innerState = 3;
		}break;
		case 3:
		{
			FUN::InitiateSelectionList(1, 6, 0x7B, 0); // Use custom population logic
			innerState = 4;
		}break;
		case 4:
		{
			uint32_t* entry = (uint32_t*)FUN::GetSelectedItem();
			if (!entry || (*entry & 0xFFF) == 0) return 0; // not ready

			uint32_t dword = *entry;
			FUN::BanishCardFromGrave(1, &dword);

			innerState = 0x5;
		}break;
		case 5:
		{
			uint16_t cardID = FUN::GetCardID(Utils::ReadUint16((void*)0x00a57802));
			FUN::ShowDialog2(0xF7);

			FUN::SetupSelector(6, cardID); // FUN_005bfa00
			FUN::InitiateSelector(); // FUN_005bfa20

			innerState = 0x6;
		}break;
		case 6:
		{
			if (FUN::SelectionConfirmed() != 0) return 0;

			uint16_t choice = Utils::ReadUint16((void*)0x00A558B4);

			uint32_t summonParam = Utils::ReadUint32((void*)0x00A55080);
			summonParam = (summonParam & 0xFFFF3FFF);
			if ((choice & 1) == 0) summonParam |= 0x4000;
			else summonParam |= 0x8000;

			Utils::WriteUint32((void*)0x00A55080, summonParam);

			Utils::WriteUint8((void*)0x00A558B4, (uint8_t)(choice & 1));

			innerState = 0xf;
		}break;
		case 0xf:
		{
			uint32_t state = Utils::ReadUint32((void*)0x00a57808);
			Utils::WriteInt32((void*)0x00a57808, state & 0xff00ffff);

			uint16_t choice = Utils::ReadUint16((void*)0x00A558B4) & 1;

			uint32_t param1 = (Utils::ReadUint32((void*)0x00a57808) >> 0x10 & 0x100) >> 8;
			uint32_t param2 = Utils::ReadUint8((void*)0x00a5780c);
			uint32_t param3 = FUN::GetSummonZone( ((Utils::ReadUint32((void*)0x00a5780a)) & 0x100) >> 8 );
			uint32_t param5 = (choice == 0) ? 1 : 0;

			FUN::SpecialSummon(param1, param2, param3, 0, param5);

			uint32_t x = Utils::ReadUint32((void*)0x00a57804);
			Utils::WriteInt32((void*)0x00a57804, x & 0xfffffffd);

			innerState = 0;
			return 1;
		}
	}

	return 0;
}
void __stdcall LoadSelectionListDark()
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