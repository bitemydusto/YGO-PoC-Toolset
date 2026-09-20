#pragma once

#include <cstdint>

// 0-4: Monster Zones
// 5-9: Spell/Trap Zones
enum Location : uint8_t
{
	FIELDZONE = 0xA,
	HAND = 0xB,
	EXTRA = 0xC,
	DECK = 0xD,
	GRAVE = 0xE,
	BANISHED = 0xF
};

namespace FUN
{
    template <typename T>
    inline T GetFunction(std::uintptr_t address)
    {
        return reinterpret_cast<T>(address);
    }

    inline auto GetMonsterType = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004025D0);

	inline auto GetCardSubType = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402740);

    inline auto GetMonsterAttribute = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402650);

	inline auto GetMonsterLevel = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004026c0);

	inline auto GetMonsterATK = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004027b0);

	inline auto GetMonsterDEF = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402800);

	inline auto GetCardID = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x004022e0);

	inline auto GetCardIntID = reinterpret_cast<uint32_t(__cdecl*)(uint16_t cardID)>(0x00402460);

	inline auto GetSpellTrapType = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x00402700);

	inline auto GetSpellSpeed = reinterpret_cast<uint32_t(__cdecl*)(uint16_t intID)>(0x0057e030);

    inline auto ShowDialog = reinterpret_cast<void(__cdecl*)(const char*)>(0x005bf860);

	inline auto ShowDialog2 = reinterpret_cast<void(__cdecl*)(unsigned int)>(0x005bf860);

	inline auto ShowDialogOptions = reinterpret_cast<void(__cdecl*)(unsigned int, unsigned int)>(0x005bfa00);

    inline auto PayLifePoints = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x005783b0);

	inline auto IsCardOnTheField = reinterpret_cast<uint32_t(__cdecl*)(uint16_t cardID)>(0x005699c0);

	// Generates target parameters and saves them to the target pointer
	inline auto SetTargetParams = reinterpret_cast<uint32_t(__cdecl*)(unsigned int block, int param2, unsigned int* target)>(0x005833e0);

    // Sends the card from the field to the destination based on the zone bit field (16 bit for each field, first 11 bits -> monster/st/fieldspell)
	// destCode: 0xb = hand, 0xd = deck, 0xe = grave, 0xf = banish
    // fxCode: bit field for effect, 0 = no effect, 2 = play destroy sound and visual effect
	inline auto SendCardFromField = reinterpret_cast<unsigned int(__cdecl*)(uint8_t * block, unsigned int zoneBitField, unsigned int destCode, unsigned int fxCode)>(0x005768b0);

	inline auto DiscardFromHand = reinterpret_cast<unsigned int(__cdecl*)(unsigned int player, unsigned int handIdx, unsigned int flag)>(0x005758c0);

	inline auto DealEffectDamage = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x00578430);

	inline auto GetCardPtrFromLocation = reinterpret_cast<uint32_t * (__cdecl*)(unsigned int playerIdx, unsigned int destCode, unsigned int idx)>(0x00570040);

	inline auto BanishFromGrave = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int* cardPtr, unsigned int flag)>(0x00575f30);

	inline auto InitiateSelectionList = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int type, unsigned int cardID, unsigned int loc)>(0x0040c990);

	inline auto PopulateSelectionList = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int cardID, unsigned int param3)>(0x00599d70);

	inline auto GetSelectionListCount = reinterpret_cast<uint32_t(__cdecl*)()>(0x0040cb30);

	inline auto SelectionConfirmed = reinterpret_cast<uint32_t(__cdecl*)()>(0x0040cac0);

	inline auto GetSelectedItem = reinterpret_cast<uint32_t(__cdecl*)()>(0x0040cb00);

	inline auto SetSelectedItemIndex = reinterpret_cast<void(__cdecl*)(unsigned int index)>(0x0040cd00);

	// Stores a 16 bit value to the target block
	// Used once for field targets, twice for full card dwords stored in lists
	inline auto StoreTarget = reinterpret_cast<void(__cdecl*)(int block, uint16_t value)>(0x00592a40);

	inline auto QueueFX = reinterpret_cast<void(__cdecl*)(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4)>(0x005b91e0);

	inline auto AddTargetedCardToHand = reinterpret_cast<void(__cdecl*)(uint8_t * block, unsigned int playerIdx, unsigned int* param3)>(0x00575ca0);

	inline auto NumOfEmptyValidSummonZones = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx)>(0x0056a000);

	inline auto NumOfTributableMonsters = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx, unsigned int param2)>(0x0056a200);

	inline auto CanPlayerSummon = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx)>(0x00570a90);

	inline auto IsCardOnField = reinterpret_cast<uint32_t(__cdecl*)(unsigned int cardID)>(0x005699c0);

	inline auto IsCardOnSideOfField = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int cardID)>(0x005699a0);

	inline auto BanishCardFromGrave = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int* cardDword)>(0x00575f30);

	inline auto SpecialSummonFromHand = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int handIdx, unsigned int desZone, unsigned int PackTributes, unsigned int pos)>(0x005adbc0);

	inline auto SpecialSummon = reinterpret_cast<void(__cdecl*)(unsigned int player, unsigned int* cardPtr, unsigned int posSelectorType, unsigned int flags, unsigned int srcLoc, unsigned int param6)>(0x005adae0);

	inline auto NormalSummon = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int handIdx, unsigned int destZone, unsigned int packedTributes, int set)>(0x005ad710);

	inline auto GetSummonZone = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx)>(0x0056a030);

	inline auto IsFieldSelectionReady = reinterpret_cast<uint32_t(__cdecl*)(unsigned int mask)>(0x005aa410);

	inline auto MarkZoneAsTributed = reinterpret_cast<void(__cdecl*)(unsigned int side, unsigned int col)>(0x00486bb0);

	inline auto IsFieldSelectionConfirmed = reinterpret_cast<uint32_t(__cdecl*)()>(0x005aa450);

	inline auto TributeSelected = reinterpret_cast<uint32_t(__cdecl*)(unsigned int side, unsigned int col)>(0x00577a80);

	inline auto IsMonsterTributable = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int sideIdx, unsigned int zoneIdx)>(0x0056a0d0);

	inline auto CopyCard = reinterpret_cast<void(__cdecl*)(void* dest, void* src)>(0x005675c0);

	inline auto PayCostToSummon = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx)>(0x0059cfe0);

	inline auto SummonMonster = reinterpret_cast<void(__cdecl*)()>(0x005ad890);

	inline auto InvokeEffect = reinterpret_cast<void(__cdecl*)(unsigned int param, unsigned int param2, unsigned int param3)>(0x005ba500);

	inline auto DrawCards = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x00578ab0);

	inline auto SelectCardsToDiscard = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount, int param3, int param4)>(0x005bce30);

	inline auto IsCardProhibited = reinterpret_cast<uint32_t(__cdecl*)(unsigned int cardIntID, unsigned int param2)>(0x0056b440);

	inline auto GetCurrentATK = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx)>(0x0056f5e0);

	inline auto GetCurrentDEF = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx)>(0x0056f600);

	inline auto AddEffectEntityToZone = reinterpret_cast<void(__cdecl*)(unsigned int packedCard, unsigned int effectIntID, uint16_t effect)>(0x0056ab60);

	inline auto IsZoneValid = reinterpret_cast<uint32_t(__cdecl*)(unsigned int side, unsigned int zoneIdx)>(0x00569e10);

	inline auto ChangeMonsterPosition = reinterpret_cast<void(__cdecl*)(uint8_t * block, unsigned int playerIdx, unsigned int zoneIdx, unsigned int set, unsigned int param5)>(0x00575130);

	inline auto ToggleMonsterPosition = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx, unsigned int toggleSet, unsigned int param4, unsigned int param5)>(0x00575160);

	inline auto ToggleFaceUp = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx, int param3, unsigned int effectIntID)>(0x00574ec0);

	inline auto FlashCardPortrait = reinterpret_cast<void(__cdecl*)(unsigned int zoneIdx, unsigned int cardIntID, unsigned int param3)>(0x005782e0);

	inline auto HasEffectEntiry = reinterpret_cast<int(__cdecl*)(unsigned int sideIdx, unsigned int zoneIdx, unsigned int effectID)>(0x0056da20);

	inline auto GetInstIndexInGrave = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx, int inst)>(0x00568ad0);

	inline auto MillCards = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount, int fxFlag)>(0x00578b00);

	inline auto CanBeSummonedByEffect = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int cardIntID)>(0x00570ac0);

	// Returns the index of the card in the spell/trap zone if it is face up, otherwise returns -1
	inline auto IsCardFaceUpInSpellTrapZones = reinterpret_cast<int(__cdecl*)(unsigned int playerIdx, unsigned int cardID)>(0x00569720);

	inline auto CanBeRevived = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* cardDWORD)>(0x00568bd0);

	inline auto CanBeRevivedFromGrave = reinterpret_cast<uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int graveIdx)>(0x00599d40);

	inline auto DiscardRandomCard = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int param2, unsigned int amount)>(0x005bcec0);

	inline auto GainLP = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int amount)>(0x00578740);

	inline auto GetRandomNumber = reinterpret_cast<int(__cdecl*)(int max)>(0x005bde20);

	inline auto SelectCardInOppHand = reinterpret_cast<void(__cdecl*)(unsigned int activatingPlayerIdx)>(0x005bcf30);

	inline auto DisCardSelectedHandIndex = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int handIdx, unsigned int param3)>(0x005758c0);

	inline auto MarkCardForFusion = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, Location location, unsigned int selectedColumn)>(0x00486bf0);

	// top = 1 -> put on top of deck
	// top = 0 -> put on bottom of deck
	inline auto PutCardFromHandToDeck = reinterpret_cast<void(__cdecl*)(unsigned int playerIdx, unsigned int handIdx, unsigned int top)>(0x005757f0);

	// SelectionType:
	// 2 = 2-way effect selection
	// 3 = 3-way effect selection
	// 4 = card type
	// 5 = attribute
	// 6 = atk/def position
	inline auto SetupSelector = reinterpret_cast<void(__cdecl*)(unsigned int selectionType, unsigned int cardID)>(0x005bfa00);

	inline auto InitiateSelector = reinterpret_cast<void(__cdecl*)()>(0x005bfa20);

	inline auto IssueCommand = reinterpret_cast<void(__cdecl*)(unsigned int cmd, unsigned int src, unsigned int dest, unsigned int param3)>(0x005b91e0);

	inline auto CanCardBeTargeted = reinterpret_cast<bool(__cdecl*)(unsigned int playerIdx, unsigned int zoneIdx)>(0x0056c510);



	using FUN_591A00_t = uint32_t(__cdecl*)(uint32_t player, uint32_t matId, uint32_t excl1, uint32_t excl2);
	using FUN_591C90_t = uint32_t(__cdecl*)(uint32_t player, uint32_t packed);
	using FUN_568580_t = int(__cdecl*)(uint32_t cardIntId);
	using FUN_56A030_t = int(__cdecl*)(uint32_t player);
	using FUN_569E10_t = int(__cdecl*)(uint32_t player, uint32_t zone);
	using FUN_56c510_t = bool(__cdecl*)(unsigned int playerIdx, int zoneIdx);
	using FUN_5777d0_t = uint32_t(__cdecl*)(uint8_t* block, unsigned int playerIdx, unsigned int zoneIdx);
	using FUN_579880_t = void(__cdecl*)(unsigned int playerIdx, unsigned int param2, unsigned int param3);
	using FUN_5b91e0_t = void(__cdecl*)(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4);

	static FUN_591A00_t  FUN_00591A00 = (FUN_591A00_t)0x00591A00;
	static FUN_591C90_t  FUN_00591C90 = (FUN_591C90_t)0x00591C90;
	static FUN_568580_t  FUN_00568580 = (FUN_568580_t)0x00568580;
	static FUN_56A030_t  FUN_0056A030 = (FUN_56A030_t)0x0056A030;
	static FUN_569E10_t  FUN_00569E10 = (FUN_569E10_t)0x00569E10;
	static FUN_56c510_t  FUN_0056C510 = (FUN_56c510_t)0x0056C510;
	static FUN_5777d0_t  FUN_005777D0 = (FUN_5777d0_t)0x005777D0;
	static FUN_579880_t  FUN_00579880 = (FUN_579880_t)0x00579880;
	static FUN_5b91e0_t  FUN_005b91e0 = (FUN_5b91e0_t)0x005b91e0;






	// Wrapers
	void W_AddEffectEntityToZone(uint32_t playerIdx, uint32_t zoneIdx, uint32_t effectIntID, uint16_t effect)
	{
		FUN::AddEffectEntityToZone((zoneIdx << 8) | playerIdx, effectIntID, effect);
	}
	uint32_t W_RollDice(uint16_t* param, uint8_t diceCmd)
	{
		int roll = FUN::GetRandomNumber(6) + 1;
		FUN::QueueFX((param[1] << 15) | diceCmd, roll, (param[1] >> 1) & 0x1F, 0);

		return roll;
	}
	void W_PutCardFromLocationToDeck(uint32_t playerIdx, Location location, uint32_t idx, bool top)
	{
		if (location < 0xa) return;
		
		uint32_t* cardDword = (uint32_t*)FUN::GetCardPtrFromLocation(playerIdx, location, idx);
		byte x = (byte)(*cardDword >> 0xc);

		FUN::QueueFX(
			0x8e,
			((((uint16_t)(char)(*cardDword >> 0x18) * 2 + (x & 1)) << 8) | (uint8_t)(char)playerIdx) & 0xff01 | 0x16,
			((((uint16_t)(top == 0)) << 8) | (uint8_t)x) & 0xff01 | 0x1a,
			0
		);
	}
	void W_PutCardFromFieldToDeck(uint32_t sideIdx, uint32_t zoneIdx, bool top)
	{
		uint32_t* cardDword = (uint32_t*)(0x00A55D64 + sideIdx * 0xd44 + 0x10 + zoneIdx * 0x90);

		byte x = (byte)(*cardDword >> 0xc);
		FUN::QueueFX(
			0x8e,
			((((uint16_t)(char)(*cardDword >> 0x18) * 2 + (x & 1)) << 8) | (uint8_t)(char)sideIdx) & 0xff01 | 0x16,
			((((uint16_t)(top == 0)) << 8) | (uint8_t)x) & 0xff01 | 0x1a,
			0
		);
	}
	uint32_t W_RollDice(uint32_t playerIdx, uint32_t sideIdx, uint8_t diceCmd)
	{
		int roll = FUN::GetRandomNumber(6) + 1;
		FUN::QueueFX((sideIdx << 15) | diceCmd, roll, (playerIdx >> 1) & 0x1F, 0);

		return roll;
	}
	void W_MoveCard(uint32_t cardDword, uint8_t _src, uint8_t _dest)
	{
		uint8_t owner = (cardDword >> 12) & 1;
		uint8_t inst = (uint8_t)(((cardDword >> 24) & 0x7F) * 2 + ((cardDword >> 12) & 1));

		uint32_t src = ((uint32_t)inst << 8) | (_src << 1) | (owner & 1);

		uint32_t dest = (_dest << 1) | (owner & 1);


		// CMD 0x8E = move
		FUN::IssueCommand(0x8E, src, dest, 0);

	}
	void W_HighlightAndStoreTarget(unsigned int param, unsigned int* entry, Location location)
	{
		uint32_t dword = *entry;
		uint8_t  owner = (dword >> 12) & 1;
		uint32_t inst = owner + ((dword >> 24) & 0x7F) * 2;
		uint32_t sideBit = owner ? 0x8000u : 0;

		uint32_t cardId = FUN::GetCardID(dword & 0xFFF);

		// Highlight / reveal
		FUN::QueueFX(sideBit | 0xDF, cardId, inst, 0);
		FUN::QueueFX(sideBit | 0x08, owner, location, 0);

		// Store targets
		FUN::StoreTarget((int)param, (uint16_t)dword);
		FUN::StoreTarget((int)param, (uint16_t)(dword >> 16));
	}
	void W_SS_HandToOpp(uint32_t handPlayer,int handIndex, uint32_t destZone, uint32_t extra,int posArg)
	{
		// This is a reimplementation of FUN_SpecialSummonFromHand, but modified so it summons to the opponent's field instead of the player's field.

		if ((int)destZone < 0) return;

		const uint32_t src = handPlayer & 1;
		const uint32_t dst = src ^ 1;

		const uint8_t posFlag = (uint8_t)(((uint32_t)(posArg == 0) << 9) >> 8);
		uint32_t mid = ((uint32_t)((posFlag << 8) | (uint8_t)handIndex) | 0x100);
		mid = (mid << 5 | (destZone & 0x1F)) << 1;

		uint32_t summonParam = Utils::ReadUint32((void*)0x00A55080);

		uint32_t kept = (summonParam & 0xFFFF4000) ^ dst;

		if (extra == 0) summonParam = mid | (kept & 0xF1FFFFFF);
		else
		{
			const uint8_t b8 = (uint8_t)(extra >> 8);
			const uint8_t b16 = (uint8_t)(extra >> 16);
			uint32_t hi =
				((((((((b16 & 0x10) << 1 | (b8 & 0x10)) << 1 | (extra & 0x10)) << 2
					| (b16 & 7)) << 2
					| (b16 & 0x80)) << 1
					| (b8 & 0x87)) << 1
					| (extra & 0x80)) << 2
					| (extra & 7));
			summonParam = (hi << 16) | mid | (kept & 0x8000FFFF);
		}

		Utils::WriteUint32((void*)0x00A55080, summonParam);

		const uintptr_t handAddr = 0x00A56434 + (uint32_t)(handIndex + (int)src * 0x351) * 4;

		const uint32_t handDword = Utils::ReadUint32((void*)handAddr);
		const uint32_t old84 = Utils::ReadUint32((void*)0x00a55084);
		Utils::WriteUint32((void*)0x00a55084, (handDword & 0xFFF) | (old84 & 0xFFFF0000));

		CopyCard((void*)0x00A55088, (void*)handAddr);

		const uint16_t old8e = Utils::ReadUint16((void*)0x00A5508E);
		Utils::WriteUint16((void*)0x00A5508E, (uint16_t)((old8e & 0xFFFB) | 0x18));
		Utils::WriteUint16((void*)0x00A5508C, 5);

		PayCostToSummon(src);

		SummonMonster();
	}

	bool W_BothLocked(uint32_t player, uint16_t a, uint16_t b)
	{
		uint32_t idA = FUN_00591C90(player, a) & 0xFFFF;
		uint32_t idB = FUN_00591C90(player, b) & 0xFFFF;
		return FUN_00568580(idA) != 0 && FUN_00568580(idB) != 0;
	}

	bool W_FieldCanFreeZone(uint32_t player, uint16_t packed)
	{
		if ((packed & 0x4000) == 0) return false;
		return FUN_00569E10(player, packed & 0xFFF) != 0;
	}

	// Effects
	inline auto DestroyEffect = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* param, int param2, int param3)>(0x00585C10);

	inline auto TargetFieldCard = reinterpret_cast<uint32_t(__cdecl*)(unsigned int* param, int param2, int param3)>(0x00596570);

	// Helper structs
	struct FieldMaskGenerator
	{
		bool zones[2][11] = {};

		uint32_t GenerateMask()
		{
			uint32_t mask = 0;
			for (int i = 0; i < 11; i++)
			{
				if (zones[0][i]) mask |= (1 << i);
				if (zones[1][i]) mask |= (1 << (i + 16));
			}
			return mask;
		}
	};
	struct Param
	{
		uint8_t* block;


		uint8_t finishedResolving;
		uint16_t cardIntID;
		uint8_t  playerIdx;
		uint8_t zoneIdx;
		uint8_t  location;
		uint32_t targetCount;
		uint16_t* fieldTargets;
		uint32_t* outerTargets;
		Param(unsigned int* param)
		{
			block = (uint8_t*)param;

			finishedResolving = block[4] & 4;
			cardIntID = *(uint16_t*)(block + 0) & 0xFFF;
			playerIdx = block[2] & 0x1;
			zoneIdx = *(uint8_t*)(block + 2) >> 1 & 0x7;
			location = (block[2] >> 1) & 0x1F;
			targetCount = *(uint16_t*)(block + 4) >> 13;
			fieldTargets = (uint16_t*)(block + 6);
			outerTargets = (uint32_t*)(block + 6);
		}
		uint8_t GetFieldTargetSide(uint8_t index)
		{
			return fieldTargets[index] & 1;
		}
		uint8_t GetFieldTargetZone(uint8_t index)
		{
			return (fieldTargets[index] >> 4) & 0xF;
		}
	};
}