#pragma once 

#include <Windows.h>
#include "Utils.h"
#include "FUN.h"

namespace GameData
{

    const uint32_t BASE_PLAYER_ADDRESS = 0x00A55D64;
    const uint32_t PLAYER_OFFSET = 0xd44;
	const uint32_t BATTLE_RESULT_ADDRESS = 0x00a57840;
    const uint32_t SELECTION_LIST_SIZE_ADDRESS = 0x00A585A4;
    const uint32_t SELECTION_LIST_ADDRESS = 0x00A582A4;
	const uint32_t EFFECT_SCRIPT_ADDRESS = 0x005ed0a8;

    struct Card
    {
        uint32_t fullValue;

		uint16_t GetIntID()
		{
			return fullValue & 0xfff;
		}
		uint16_t GetCardID()
		{
			return FUN::GetCardID(GetIntID());
		}
		uint8_t GetSubType()
		{
			return FUN::GetCardSubType(GetIntID());
		}
		uint8_t GetType()
		{
			return FUN::GetMonsterType(GetIntID());
		}
		uint8_t GetAttribute()
		{
			return FUN::GetMonsterAttribute(GetIntID());
		}
		uint8_t GetLevel()
		{
			return FUN::GetMonsterLevel(GetIntID());
		}
		uint8_t GetSpellTrapType()
		{
			return FUN::GetSpellTrapType(GetIntID());
		}
		uint16_t GetATK()
		{
			return FUN::GetMonsterATK(GetIntID());
		}
		uint16_t GetDEF()
		{
			return FUN::GetMonsterDEF(GetIntID());
		}
		uint8_t GetOwner()
		{
			return (fullValue >> 12) & 1;
		}
		uint32_t GetInstance()
		{
			return (fullValue >> 12 & 1) + (fullValue >> 24 & 0x7F) * 2;
		}
		bool WasProperlySummoned()
		{
			return (fullValue >> 0xe & 1) != 0;
		}
    };
    struct EffectEntity
    {
        uint8_t type;
        uint8_t value;
    };
    struct CardZone
    {
        Card card;
		uint16_t unknown1;
        uint16_t status;
		uint16_t unknown2;
        uint16_t effectCount;
        uint16_t effectIDs[32];
        EffectEntity effectEntries[32];
        uint32_t stateFlags;

		bool IsFaceUp()
		{
			return (status & 2) != 0;
		}
		bool InAttackPosition()
		{
			return (status & 1) == 0;
		}
    };
    struct Player
    {
        uint16_t lifePoints;
        uint8_t unkown1;
		uint8_t unkown2;
        uint8_t cardsInHand;
        uint8_t cardsInDeck;
        uint8_t cardsInGrave;
        uint8_t cardsInExtra;
        uint8_t cardsInBanish;
        uint8_t status;
        uint16_t canAttackZones;
		uint16_t alreadyAttackedZones;

        uint16_t padding;

        CardZone monsterZones[5];
        CardZone spellTrapZones[5];
		CardZone fieldSpellZone;
        uint8_t playerFlags[0x90];

        Card hand[80];
        Card deck[80];
        Card extra[15];
        Card grave[95];
        Card banish[95];

		uint8_t footer[0xc0];

        Card fieldSpell()
        {
			return fieldSpellZone.card;
        }
    };
    struct Duel
    {
        Player players[2];
    };
	Duel* _duel = reinterpret_cast<Duel*>(BASE_PLAYER_ADDRESS);
    Duel* GetDuel()
    {
        return _duel;
    }


    void ChangeSelectionList(std::vector<uint32_t> items, uint8_t loc)
    {
        Utils::WriteUint8((void*)SELECTION_LIST_SIZE_ADDRESS, items.size());
        for (size_t i = 0; i < items.size(); i++)
        {
            Utils::WriteUint32((void*)(SELECTION_LIST_ADDRESS + (i * 4)), items[i]);
            Utils::WriteUint16((void*)(0x00a584a4 + (i * 2)), loc);
        }

    }

	Utils::EffectScript GetEffectScript(int index)
	{
        Utils::EffectScript script;

		script.CardID = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript))));
		script.Effect = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 4));
		script.AppliesTo = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 8));
		script.Condition = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 12));
		script.Cost = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 16));
		script.Target = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 20));

		return script;
	}
    int GetEffectScriptIndex(uint32_t cardID)
    {
        for (int i = 0; i < 443; i++)
        {
            Utils::EffectScript script = GetEffectScript(i);
            if (script.CardID == cardID)
            {
                return i;
            }
        }
        return -1;
    }
	void SetEffectScript(int index, Utils::EffectScript script)
	{
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript))), script.CardID);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 4), script.Effect);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 8), script.AppliesTo);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 12), script.Condition);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 16), script.Cost);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(Utils::EffectScript)) + 20), script.Target);
	}
    struct BattleResultSide
    {
        uint16_t ResultFlags;
        uint16_t IntID;
        uint32_t ATK;
		uint32_t DEF;
        uint32_t DamageTaken;
    };
	struct BattleResult
	{
        uint32_t StateFlags;
		BattleResultSide sides[2];

		uint8_t GetSide(uint8_t index)
		{
			// 0 = attacker, 1 = defender
			return StateFlags >> index & 1;
		}
		uint8_t GetZone(uint8_t index)
		{
			// 0 = attacker, 1 = defender
			return StateFlags >> (8 + index * 3) & 7;
		}
	};

	BattleResult* _battleResult = reinterpret_cast<BattleResult*>(BATTLE_RESULT_ADDRESS);
	BattleResult* GetBattleResult()
	{
		return _battleResult;
	}

    uint16_t GetSummonState()
    {
		return Utils::ReadUint16((void*)(0x00a57808));
    }
    void SetSummonState(uint16_t state)
    {
        Utils::WriteUint16((void*)(0x00a57808), state);
    }

    // Used by effect functions
    // Starts at 0x80 and changes to what the effect function returns
    // When the effect function returns 0, it is finished resolving
	uint8_t GetEffectState()
	{
		return Utils::ReadUint8((void*)(0x00a55c88 + 2));
	}
	// Used by target and cost functions
    // Starts at 0 and must be set manually
	// Make sure to set it back to 0 when the function is done
	uint8_t GetEffectSubState()
	{
		return Utils::ReadUint8((void*)(0x00a55c8e));
	}
	void SetEffectSubState(uint8_t subState)
	{
		Utils::WriteUint8((void*)(0x00a55c8e), subState);
	}

    uint8_t GetDialogResult()
    {
        return Utils::ReadUint8((void*)0x00a558b4);
    }
    // Returns the intID of the card last used/summoned
    uint16_t GetCardUsed()
    {
		return Utils::ReadUint16((void*)0x00a57802) & 0xfff;
    }
    uint8_t GetSelectedSide()
    {
        return Utils::ReadUint8((void*)0x000a55044);
    }
	uint8_t GetSelectedColumn()
	{
		return Utils::ReadUint8((void*)0x00a5504c);
	}
	// 0x0 : Monster Zone
	// 0x5 : Spell/Trap Zone
	// 0xa : Field Zone
	// 0xb : Hand
	// 0xc : Extra
	// 0xd : Deck
	// 0xe : Graveyard
	// 0xf : Banish
    uint8_t GetSelectedLocation()
    {
		return Utils::ReadUint8((void*)0x00a55048);
    }
	uint8_t GetSelectedZone()
	{
		uint8_t loc = GetSelectedLocation();

		if (loc > 9) return loc;
		return GetSelectedColumn() + loc;
	}
	// lower byte = main state
	// upper byte = summon state
    uint16_t GetState()
    {
       return Utils::ReadUint16((void*)0x00a57808);
    }
	void SetState(uint16_t state)
	{
		Utils::WriteUint16((void*)0x00a57808, state);
	}
	uint16_t GetSelectedSoFar()
	{
		return Utils::ReadUint16((void*)0x00a5780a);
	}
	void SetSelectedSoFar(uint16_t value)
	{
		Utils::WriteUint16((void*)0x00a5780a, value);
	}
    uint32_t GetSummonParam()
    {
        return Utils::ReadUint32((void*)0x00A55080);
    }
	void SetSummonParam(uint32_t param)
	{
		Utils::WriteUint32((void*)0x00A55080, param);
	}
	uint8_t GetTurnPlayer()
	{
		return Utils::ReadUint8((void*)0x00a577fa) & 1;
	}
	uint8_t GetLocalSide() {
		return Utils::ReadUint8((void*)0x00A54E5C) & 1;
	}
    uint16_t GetSelectedHandIndex()
    {
        return Utils::ReadUint16((void*)0x00a55064);
    }
	uint16_t GetConfirmedHandIndex()
	{
		return Utils::ReadUint16((void*)0x00a57822);
	}
	uint8_t GetPhase()
	{
		return Utils::ReadUint8((void*)0x00a577fa) >> 1;
	}

	struct Combination
	{
		uint8_t side;
		uint8_t mask;
		vector<uint16_t> cards;

		Combination(uint8_t _side, uint8_t _mask)
		{
			side = _side;
			mask = _mask;

			auto player = GameData::GetDuel()->players[side];
			for (uint8_t i = 0; i < 5; i++)
			{
				if (mask & (1 << i))
				{
					uint16_t intID = player.monsterZones[i].card.GetIntID();
					if (intID != 0)
					{
						cards.push_back(player.monsterZones[i].card.GetCardID());
					}
				}
			}
		}
		bool operator==(const Combination& other) const
		{
			return side == other.side && mask == other.mask;
		}
		uint8_t GetSize()
		{
			return cards.size();
		}
		uint8_t GetNumOfTuners()
		{
			uint8_t count = 0;
			for (auto card : cards)
			{
				if (IsTunerMonster(card)) count++;
			}
			return count;
		}
		uint8_t GetCombinedLevel()
		{
			uint8_t level = 0;
			for (auto card : cards)
			{
				level += FUN::GetMonsterLevel(card);
			}
			return level;
		}
	};
	// HOW TO USE:
	// Upon creating an instance of MaterialCombinator, it will automatically generate all possible combinations of monsters on the field
	// for the specified side (0 or 1). You can then access the combinations vector to retrieve the generated combinations and their properties,
	// such as size, number of tuners, and combined level. Use these or your own logic to filter the combinations as needed for your
	// fusion/synchro etc. summoning or other card effects.
	struct MaterialCombinator
	{
		vector<Combination> combinations;

		MaterialCombinator(uint8_t side)
		{
			auto player = GameData::GetDuel()->players[side];

			// Get all possible combinations of monsters on the field
			// Generate all combinations using bitmasking (1 to 31 = 2^5 - 1)
			// For example, zone 0 and 1 = 00011 = 3, zone 0,2 and 3 = 01101 = 13, etc.
			for (uint8_t mask = 1; mask < 32; mask++)
			{
				Combination combo(side, mask);


				if (combo.GetSize() >= 2)
				{
					combinations.push_back(combo);
				}
			}
		}
		void Filter(bool(*filterFunction)(Combination combo))
		{
			for (auto& item : combinations)
			{
				if (!filterFunction(item))
				{
					combinations.erase(std::remove(combinations.begin(), combinations.end(), item), combinations.end());
				}
			}
		}
	};

}