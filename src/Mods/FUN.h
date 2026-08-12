#pragma once

#include <cstdint>

namespace FUN
{
    template <typename T>
    inline T GetFunction(std::uintptr_t address)
    {
        return reinterpret_cast<T>(address);
    }

    using GetMonsterType_t = uint32_t(__cdecl*)(uint16_t intID);
    inline GetMonsterType_t GetMonsterType = reinterpret_cast<GetMonsterType_t>(0x004025D0);

	using GetCardSubType_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetCardSubType_t GetCardSubType = reinterpret_cast<GetCardSubType_t>(0x00402740);

    using GetMonsterAttribute_t = uint32_t(__cdecl*)(uint16_t intID);
    inline GetMonsterAttribute_t GetMonsterAttribute = reinterpret_cast<GetMonsterAttribute_t>(0x00402650);

	using GetMonsterLevel_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetMonsterLevel_t GetMonsterLevel = reinterpret_cast<GetMonsterLevel_t>(0x004026c0);

	using GetMonsterATK_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetMonsterATK_t GetMonsterATK = reinterpret_cast<GetMonsterATK_t>(0x004027b0);

	using GetMonsterDEF_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetMonsterDEF_t GetMonsterDEF = reinterpret_cast<GetMonsterDEF_t>(0x00402800);

	using GetCardID_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetCardID_t GetCardID = reinterpret_cast<GetCardID_t>(0x004022e0);

	using GetCardIntID_t = uint32_t(__cdecl*)(uint16_t cardID);
	inline GetCardIntID_t GetCardIntID = reinterpret_cast<GetCardIntID_t>(0x00402460);

	using GetSpellTrapType_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetSpellTrapType_t GetSpellTrapType = reinterpret_cast<GetSpellTrapType_t>(0x00402700);

	using GetSpellSpeed_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetSpellSpeed_t GetSpellSpeed = reinterpret_cast<GetSpellSpeed_t>(0x0057e030);

    using ShowDialog_t = void(__cdecl*)(const char*);
    inline ShowDialog_t ShowDialog = reinterpret_cast<ShowDialog_t>(0x005bf860);

	using ShowDialog2_t = void(__cdecl*)(unsigned int);
	inline ShowDialog2_t ShowDialog2 = reinterpret_cast<ShowDialog2_t>(0x005bf860);

	using ShowDialogOptions_t = void(__cdecl*)(unsigned int, unsigned int);
	inline ShowDialogOptions_t ShowDialogOptions = reinterpret_cast<ShowDialogOptions_t>(0x005bfa00);

	using PayLifePoints_t = void(__cdecl*)(unsigned int playerIdx, unsigned int amount);
    inline PayLifePoints_t PayLifePoints = reinterpret_cast<PayLifePoints_t>(0x005783b0);

	using IsCardOnTheField_t = uint32_t(__cdecl*)(uint16_t cardID);
	inline IsCardOnTheField_t IsCardOnTheField = reinterpret_cast<IsCardOnTheField_t>(0x005699c0);

	// Generates target parameters and saves them to the target pointer
	using GetTargetParams_t = uint32_t(__cdecl*)(unsigned int block, int param2, unsigned int* target);
	inline GetTargetParams_t SetTargetParams = reinterpret_cast<GetTargetParams_t>(0x005833e0);

    // Sends the card from the field to the destination based on the zone bit field (16 bit for each field, first 11 bits -> monster/st/fieldspell)
	// destCode: 0xb = hand, 0xd = deck, 0xe = grave, 0xf = banish
    // fxCode: bit field for effect, 0 = no effect, 2 = play destroy sound and visual effect
	using SendCardFromField_t = unsigned int(__cdecl*)(uint8_t* block, unsigned int zoneBitField, unsigned int destCode, unsigned int fxCode);
	inline SendCardFromField_t SendCardFromField = reinterpret_cast<SendCardFromField_t>(0x005768b0);

	using DiscardFromHand_t = unsigned int(__cdecl*)(unsigned int player, unsigned int handIdx, unsigned int flag);
	inline DiscardFromHand_t DiscardFromHand = reinterpret_cast<DiscardFromHand_t>(0x005758c0);

	using DealEffectDamage_t = void(__cdecl*)(unsigned int playerIdx, unsigned int amount);
	inline DealEffectDamage_t DealEffectDamage = reinterpret_cast<DealEffectDamage_t>(0x00578430);

	using GetCardPtrFromDest_t = uint32_t*(__cdecl*)(unsigned int playerIdx, unsigned int destCode, unsigned int idx);
	inline GetCardPtrFromDest_t GetCardPtrFromDest = reinterpret_cast<GetCardPtrFromDest_t>(0x00570040);

	using BanishFromGrave_t = void(__cdecl*)(unsigned int playerIdx, unsigned int* cardPtr, unsigned int flag);
	inline BanishFromGrave_t BanishFromGrave = reinterpret_cast<BanishFromGrave_t>(0x00575f30);

	using InitiateSelectionList_t = void(__cdecl*)(unsigned int playerIdx, unsigned int param2, unsigned int cardID, unsigned int param4);
	inline InitiateSelectionList_t InitiateSelectionList = reinterpret_cast<InitiateSelectionList_t>(0x0040c990);

	using GetSelectionListCount_t = uint32_t(__cdecl*)();
	inline GetSelectionListCount_t GetSelectionListCount = reinterpret_cast<GetSelectionListCount_t>(0x0040cb30);

	using SelectionConfirmed_t = uint32_t(__cdecl*)();
	inline SelectionConfirmed_t SelectionConfirmed = reinterpret_cast<SelectionConfirmed_t>(0x0040cac0);

	using GetSelectedItem_t = uint32_t(__cdecl*)();
	inline GetSelectedItem_t GetSelectedItem = reinterpret_cast<GetSelectedItem_t>(0x0040cb00);

	using SetSelectedItemIndex_t = void(__cdecl*)(unsigned int index);
	inline SetSelectedItemIndex_t SetSelectedItemIndex = reinterpret_cast<SetSelectedItemIndex_t>(0x0040cd00);

	using FUN_00592a40_t = void(__cdecl*)(int block, uint16_t value);
	inline FUN_00592a40_t FUN_00592a40 = reinterpret_cast<FUN_00592a40_t>(0x00592a40);

	using HighLightCard_t = void(__cdecl*)(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
	inline HighLightCard_t HighLightCard = reinterpret_cast<HighLightCard_t>(0x005b91e0);

	using AddTargetedCardToHand_t = void(__cdecl*)(uint8_t* block, unsigned int playerIdx, unsigned int* param3);
	inline AddTargetedCardToHand_t AddTargetedCardToHand = reinterpret_cast<AddTargetedCardToHand_t>(0x00575ca0);

	using NumOfEmptyValidSummonZones_t = uint32_t(__cdecl*)(unsigned int playerIdx);
	inline NumOfEmptyValidSummonZones_t NumOfEmptyValidSummonZones = reinterpret_cast<NumOfEmptyValidSummonZones_t>(0x0056a000);

	using CanPlayerSummon_t = uint32_t(__cdecl*)(unsigned int playerIdx);
	inline CanPlayerSummon_t CanPlayerSummon = reinterpret_cast<CanPlayerSummon_t>(0x00570a90);

	using IsCardOnField_t = uint32_t(__cdecl*)(unsigned int cardID);
	inline IsCardOnField_t IsCardOnField = reinterpret_cast<IsCardOnField_t>(0x005699c0);

	using IsCardOnSideOfField_t = uint32_t(__cdecl*)(unsigned int playerIdx, unsigned int cardID);
	inline IsCardOnSideOfField_t IsCardOnSideOfField = reinterpret_cast<IsCardOnSideOfField_t>(0x005699a0);

	using BanishCardFromGrave_t = void(__cdecl*)(unsigned int playerIdx, unsigned int* cardDword);
	inline BanishCardFromGrave_t BanishCardFromGrave = reinterpret_cast<BanishCardFromGrave_t>(0x00575f30);

	using SpecialSummonFromHand_t = void(__cdecl*)(unsigned int param1, unsigned int param2, unsigned int param3, unsigned int param4, unsigned int param5);
	inline SpecialSummonFromHand_t SpecialSummonFromHand = reinterpret_cast<SpecialSummonFromHand_t>(0x005adbc0);

	using SpecialSummon_t = void(__cdecl*)(unsigned int player, unsigned int* cardPtr, unsigned int posSelectorType, unsigned int flags, unsigned int srcLoc, unsigned int param6);
	inline SpecialSummon_t SpecialSummon = reinterpret_cast<SpecialSummon_t>(0x005adae0);

	using GetSummonZone_t = uint32_t(__cdecl*)(unsigned int param1);
	inline GetSummonZone_t GetSummonZone = reinterpret_cast<GetSummonZone_t>(0x0056a030);

	using IsFieldSelectionReady_t = uint32_t(__cdecl*)(unsigned int mask);
	inline IsFieldSelectionReady_t IsFieldSelectionReady = reinterpret_cast<IsFieldSelectionReady_t>(0x005aa410);

	using MarkZoneAsTributed_t = void(__cdecl*)(unsigned int side, unsigned int col);
	inline MarkZoneAsTributed_t MarkZoneAsTributed = reinterpret_cast<MarkZoneAsTributed_t>(0x00486bb0);

	using IsFieldSelectionConfirmed_t = uint32_t(__cdecl*)();
	inline IsFieldSelectionConfirmed_t IsFieldSelectionConfirmed = reinterpret_cast<IsFieldSelectionConfirmed_t>(0x005aa450);

	using TributeSelected_t = uint32_t(__cdecl*)(unsigned int side, unsigned int col);
	inline TributeSelected_t TributeSelected = reinterpret_cast<TributeSelected_t>(0x00577a80);

	using IsMonsterTributable_t = uint32_t(__cdecl*)(unsigned int sideIdx, unsigned int playerIdx, unsigned int zoneIdx);
	inline IsMonsterTributable_t IsMonsterTributable = reinterpret_cast<IsMonsterTributable_t>(0x0056a0d0);

	using CopyCard_t = void(__cdecl*)(void* dest, void* src);
	inline CopyCard_t CopyCard = reinterpret_cast<CopyCard_t>(0x005675c0);

	using PayCostToSummon_t = void(__cdecl*)(unsigned int playerIdx);
	inline PayCostToSummon_t PayCostToSummon = reinterpret_cast<PayCostToSummon_t>(0x0059cfe0);

	using SummonMonster_t = void(__cdecl*)();
	inline SummonMonster_t SummonMonster = reinterpret_cast<SummonMonster_t>(0x005ad890);

	using InvokeEffect_t = void(__cdecl*)(unsigned int param, unsigned int param2, unsigned int param3);
	inline InvokeEffect_t InvokeEffect = reinterpret_cast<InvokeEffect_t>(0x005ba500);


	// SelectionType:
	// 4 = card type
	// 5 = attribute
	// 6 = atk/def position
	using SetupSelector_t = void(__cdecl*)(unsigned int selectionType, unsigned int cardID);
	inline SetupSelector_t SetupSelector = reinterpret_cast<SetupSelector_t>(0x005bfa00);

	using InitiateSelector_t = void(__cdecl*)();
	inline InitiateSelector_t InitiateSelector = reinterpret_cast<InitiateSelector_t>(0x005bfa20);

	using IssueCommand_t = void(__cdecl*)(unsigned int cmd, unsigned int src, unsigned int dest, unsigned int param3);
	inline IssueCommand_t IssueCommand = reinterpret_cast<IssueCommand_t>(0x005b91e0);






	// Wrapers
	void W_MoveCard(uint32_t cardDword, uint8_t _src, uint8_t _dest)
	{
		uint8_t owner = (cardDword >> 12) & 1;
		uint8_t inst = (uint8_t)(((cardDword >> 24) & 0x7F) * 2 + ((cardDword >> 12) & 1));

		uint32_t src = ((uint32_t)inst << 8) | (_src << 1) | (owner & 1);

		uint32_t dest = (_dest << 1) | (owner & 1);


		// CMD 0x8E = move
		FUN::IssueCommand(0x8E, src, dest, 0);

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