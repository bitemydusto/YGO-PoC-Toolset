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

	using GetCardID_t = uint32_t(__cdecl*)(uint32_t intID);
	inline GetCardID_t GetCardID = reinterpret_cast<GetCardID_t>(0x004022e0);

	using GetSpellTrapType_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetSpellTrapType_t GetSpellTrapType = reinterpret_cast<GetSpellTrapType_t>(0x00402700);

	using GetSpellSpeed_t = uint32_t(__cdecl*)(uint16_t intID);
	inline GetSpellSpeed_t GetSpellSpeed = reinterpret_cast<GetSpellSpeed_t>(0x0057e030);

    using ShowDialog_t = void(__cdecl*)(const char*);
    inline ShowDialog_t ShowDialog = reinterpret_cast<ShowDialog_t>(0x005bf860);

	using ShowDialogOptions_t = void(__cdecl*)(unsigned int, unsigned int);
	inline ShowDialogOptions_t ShowDialogOptions = reinterpret_cast<ShowDialogOptions_t>(0x005bfa00);

	using PayLifePoints_t = void(__cdecl*)(unsigned int playerIdx, unsigned int amount);
    inline PayLifePoints_t PayLifePoints = reinterpret_cast<PayLifePoints_t>(0x005783b0);

	using IsCardOnTheField_t = uint32_t(__cdecl*)(uint16_t cardID);
	inline IsCardOnTheField_t IsCardOnTheField = reinterpret_cast<IsCardOnTheField_t>(0x005699c0);

	// Generates target parameters and saves them to the target pointer
	using GetTargetParams_t = uint32_t(__cdecl*)(unsigned int block, int param2, unsigned int* target);
	inline GetTargetParams_t SetTargetParams = reinterpret_cast<GetTargetParams_t>(0x005833e0);

    // Sends the card from the field to the destination based on the zone bit field (16 bit for each field, first 11 bits -> monster/st/field spell)
	// destCode: 0xb = hand, 0xd = deck, 0xe = grave, 0xf = banish
    // fxCode: bit field for effect, 0 = no effect, 2 = play destroy sound and visual effect
	using SendCardFromField_t = unsigned int(__cdecl*)(uint8_t* block, unsigned int zoneBitField, unsigned int destCode, unsigned int fxCode);
	inline SendCardFromField_t SendCardFromField = reinterpret_cast<SendCardFromField_t>(0x005768b0);

	using DiscardFromHand_t = unsigned int(__cdecl*)(unsigned int player, unsigned int handIdx, unsigned int flag);
	inline DiscardFromHand_t DiscardFromHand = reinterpret_cast<DiscardFromHand_t>(0x005758c0);

	using DealEffectDamage_t = void(__cdecl*)(unsigned int playerIdx, unsigned int amount);
	inline DealEffectDamage_t DealEffectDamage = reinterpret_cast<DealEffectDamage_t>(0x00578430);

	using GetCardPtrFromGrave_t = uint32_t*(__cdecl*)(unsigned int playerIdx, unsigned int destCode, unsigned int graveIdx);
	inline GetCardPtrFromGrave_t GetCardPtrFromGrave = reinterpret_cast<GetCardPtrFromGrave_t>(0x00570040);

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

	using FUN_00592a40_t = void(__cdecl*)(int block, uint16_t value);
	inline FUN_00592a40_t FUN_00592a40 = reinterpret_cast<FUN_00592a40_t>(0x00592a40);

	using HighLightCard_t = void(__cdecl*)(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
	inline HighLightCard_t HighLightCard = reinterpret_cast<HighLightCard_t>(0x005b91e0);

	using AddTargetedCardToHand_t = void(__cdecl*)(uint8_t* block, unsigned int playerIdx, unsigned int* param3);
	inline AddTargetedCardToHand_t AddTargetedCardToHand = reinterpret_cast<AddTargetedCardToHand_t>(0x00575ca0);
}