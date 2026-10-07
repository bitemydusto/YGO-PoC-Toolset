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
		uint32_t GetPack(uint8_t side, uint8_t zone)
		{
			((uint32_t)(zone & 0x1F) | ((uint32_t)side << 0xf) | 0x0A20u) << 16 | GetIntID();
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
		uint8_t GetTurnCounter()
		{
			return (status & 0x3C) >> 2;
		}
		uint8_t GetDeathCounter()
		{
			return (status & 0x1C0) >> 6;
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

        CardZone cardZones[11];
		CardZone playerZone;

        Card hand[80];
        Card deck[80];
        Card extra[15];
        Card grave[95];
        Card banish[95];

		uint8_t footer[0xc0];

		CardZone monsterZone(uint8_t index)
		{
			if (index > 4) return cardZones[4];
			return cardZones[index];
		}
		CardZone spellTrapZone(uint8_t index)
		{
			if (index < 5) return cardZones[5];
			if (index > 9) return cardZones[9];
			return cardZones[index];
		}
		CardZone fieldSpellZone()
        {
			return cardZones[10];
        }
    };
    struct Duel
    {
        Player players[2];
    };
	inline Duel* _duel = reinterpret_cast<Duel*>(BASE_PLAYER_ADDRESS);
    inline Duel* GetDuel()
    {
        return _duel;
    }

	inline void ChangeSelectionList(std::vector<uint32_t> items, uint8_t loc)
    {
        Utils::WriteUint8((void*)SELECTION_LIST_SIZE_ADDRESS, items.size());
        for (size_t i = 0; i < items.size(); i++)
        {
            Utils::WriteUint32((void*)(SELECTION_LIST_ADDRESS + (i * 4)), items[i]);
            Utils::WriteUint16((void*)(0x00a584a4 + (i * 2)), loc);
        }

    }

	inline Utils::EffectScript GetEffectScript(int index)
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
	inline int GetEffectScriptIndex(uint32_t cardID)
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
	inline void SetEffectScript(int index, Utils::EffectScript script)
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

	inline BattleResult* _battleResult = reinterpret_cast<BattleResult*>(BATTLE_RESULT_ADDRESS);
	inline BattleResult* GetBattleResult()
	{
		return _battleResult;
	}

	inline uint16_t GetSummonState()
    {
		return Utils::ReadUint16((void*)(0x00a57808));
    }
	inline void SetSummonState(uint16_t state)
    {
        Utils::WriteUint16((void*)(0x00a57808), state);
    }

    // Used by effect functions
    // Starts at 0x80 and changes to what the effect function returns
    // When the effect function returns 0, it is finished resolving
	inline uint8_t GetEffectState()
	{
		return Utils::ReadUint8((void*)(0x00a55c88 + 2));
	}
	// Used by target and cost functions
    // Starts at 0 and must be set manually
	// Make sure to set it back to 0 when the function is done
	inline uint8_t GetEffectSubState()
	{
		return Utils::ReadUint8((void*)(0x00a55c8e));
	}
	inline void SetEffectSubState(uint8_t subState)
	{
		Utils::WriteUint8((void*)(0x00a55c8e), subState);
	}

	inline uint8_t GetDialogResult()
    {
        return Utils::ReadUint8((void*)0x00a558b4);
    }
    // Returns the intID of the card last used/summoned
	inline uint16_t GetCardUsed()
    {
		return Utils::ReadUint16((void*)0x00a57802) & 0xfff;
    }
	inline uint8_t GetSelectedSide()
    {
        return Utils::ReadUint8((void*)0x000a55044);
    }
	inline uint8_t GetSelectedColumn()
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
	inline uint8_t GetSelectedLocation()
    {
		return Utils::ReadUint8((void*)0x00a55048);
    }
	inline uint8_t GetSelectedZone()
	{
		uint8_t loc = GetSelectedLocation();

		if (loc > 9) return loc;
		return GetSelectedColumn() + loc;
	}
	// lower byte = main state
	// upper byte = summon state
	inline uint16_t GetState()
    {
       return Utils::ReadUint16((void*)0x00a57808);
    }
	inline void SetState(uint16_t state)
	{
		Utils::WriteUint16((void*)0x00a57808, state);
	}
	inline uint16_t GetSelectedSoFar()
	{
		return Utils::ReadUint16((void*)0x00a5780a);
	}
	inline void SetSelectedSoFar(uint16_t value)
	{
		Utils::WriteUint16((void*)0x00a5780a, value);
	}
	inline uint32_t GetSummonParam()
    {
        return Utils::ReadUint32((void*)0x00A55080);
    }
	inline void SetSummonParam(uint32_t param)
	{
		Utils::WriteUint32((void*)0x00A55080, param);
	}
	inline uint8_t GetTurnPlayer()
	{
		return Utils::ReadUint8((void*)0x00a577fa) & 1;
	}
	inline uint8_t GetLocalSide() {
		return Utils::ReadUint8((void*)0x00A54E5C) & 1;
	}
	inline uint16_t GetSelectedHandIndex()
    {
        return Utils::ReadUint16((void*)0x00a55064);
    }
	inline uint16_t GetConfirmedHandIndex()
	{
		return Utils::ReadUint16((void*)0x00a57822);
	}
	inline uint8_t GetPhase()
	{
		return Utils::ReadUint8((void*)0x00a577fa) >> 1;
	}
	inline bool IsAI(uint8_t side)
	{
		return ((*(uint8_t*)0x00A54E5C >> 1) >> (side & 1) & 1) != 0;
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
				if (ZoneIncluded(i)) // Is this zone included in the combination?
				{
					auto zone = player.cardZones[i];
					if (zone.card.GetCardID() == 0 || !zone.IsFaceUp())
					{
						cards = {}; // If the zone is included but there is no face-up monster in it, this combination is invalid, so clear the cards vector and return
						return;
					}
					else
					{
						cards.push_back(player.cardZones[i].card.GetCardID());
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
				level += FUN::GetMonsterLevel(FUN::GetCardIntID(card));
			}
			return level;
		}
		bool ZoneIncluded(uint8_t zone)
		{
			return (mask & (1 << zone)) != 0;
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
		bool UseMaterial(uint8_t zone)
		{
			bool done = false;

			combinations.erase(std::remove_if(combinations.begin(),combinations.end(),
					[&](Combination& combo)
					{
						if (!combo.ZoneIncluded(zone)) return true;
						combo.mask &= ~(1 << zone);

						if (combo.mask == 0)
						{
							done = true;
							return true;
						}

						return false;
					}),combinations.end());

			return done;
		}
		bool ZoneValid(uint8_t zone)
		{
			for (auto& combo : combinations)
			{
				if (combo.ZoneIncluded(zone))
				{
					return true;
				}
			}
			return false;
		}
	};
	struct SynchroSummoner
	{
		MaterialCombinator combinator;
		uint8_t level;
		uint16_t cardID;
		int innerState = 0;

		SynchroSummoner(uint8_t side, uint16_t _cardID) : combinator(side), cardID(_cardID)
		{
			level = FUN::GetMonsterLevel(FUN::GetCardIntID(cardID));
			combinator.combinations.erase(std::remove_if(combinator.combinations.begin(), combinator.combinations.end(),
				[&](Combination& combo)
				{
					if (combo.GetCombinedLevel() != level || combo.GetNumOfTuners() != 1) return true;

					return false;
				}), combinator.combinations.end());
		}
		bool Standard_Synchro_Condition(uint8_t side)
		{
			for (auto& combo : combinator.combinations)
			{
				if (combo.GetCombinedLevel() == level && combo.GetNumOfTuners() == 1)
				{
					return true;
				}
			}

			return false;
		}
		uint32_t __stdcall Standard_Synchro_SummonState()
		{
			switch (innerState)
			{
				case 0:
				{
					FUN::ShowDialog("Select the listed materials on your side of the field.");
					innerState = 1;
				}break;
				case 1:
				{
					uint8_t side = GameData::GetSelectedSide();
					uint8_t zone = GameData::GetSelectedColumn() + GameData::GetSelectedLocation();

					if (side != GameData::GetTurnPlayer() || zone > 4) return 0;

					uint16_t intID = _duel->players[side].cardZones[zone].card.GetIntID();
					if (intID == 0) return 0;

					if (!combinator.ZoneValid(zone)) return 0;

					if (FUN::IsFieldSelectionConfirmed() == 0) return 0;

					bool done = combinator.UseMaterial(zone);

					FUN::FieldMaskGenerator maskGen;
					maskGen.zones[side][zone] = true;

					uint8_t block[32] = {};
					FUN::SendCardFromField(block, maskGen.GenerateMask(), 0xe, 0);

					if (done) innerState = 2;
				}break;
				case 2:
				{
					uint32_t* cardDword = nullptr;
					for (size_t i = 0; i < _duel->players[GameData::GetTurnPlayer()].cardsInExtra; i++)
					{
						uint16_t id = FUN::GetCardID(_duel->players[GameData::GetTurnPlayer()].extra[i].GetIntID());
						if (id == cardID)
						{
							cardDword = &(_duel->players[GameData::GetTurnPlayer()].extra[i].fullValue);
							break;
						}
					}

					FUN::SpecialSummon(1, cardDword, 1, 1, 0x0C, 0);

					innerState = 0;
					return 1;
				}
			}

			return 0;
		}
	};
}