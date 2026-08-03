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

    using ShowDialogue_t = void(__cdecl*)(const char*);
    inline ShowDialogue_t ShowDialogue = reinterpret_cast<ShowDialogue_t>(0x005bf860);

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

}