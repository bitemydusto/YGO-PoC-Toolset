#pragma once 

#include <Windows.h>
#include "Utils.h"
#include "FUN.h"

namespace GameData
{

    const uint32_t BASE_PLAYER_ADDRESS = 0x00A55D64;
    const uint32_t PLAYER_OFFSET = 0xd44;
    const uint32_t SELECTION_LIST_SIZE_ADDRESS = 0x00A585A4;
    const uint32_t SELECTION_LIST_ADDRESS = 0x00A582A4;
	const uint32_t EFFECT_SCRIPT_ADDRESS = 0x005ed0a8;

    struct Card
    {
        uint32_t fullValue;
        uint16_t intID;

        uint32_t GetAttribute()
        {
            return FUN::GetMonsterAttribute(intID);
        }
        uint32_t GetType()
        {
            return FUN::GetMonsterType(intID);
        }
    };
    struct EffectEntry
    {
        uint8_t type;
        uint8_t value;
    };
    struct MonsterZone
    {
        Card card;
        uint16_t effectCount;
        uint16_t effectIDs[32];
        EffectEntry effectEntries[32];
        uint32_t stateFlags;
    };
    struct SpellTrapZone
    {
        Card card;
    };
    struct Player
    {
        int id;
        uint16_t lifePoints;
        uint8_t cardsInHand;
        uint8_t cardsInDeck;
        uint8_t cardsInGrave;
        uint8_t cardsInExtra;
        uint8_t cardsInBanish;
        uint8_t status;
        uint16_t canAttackZones;
		uint16_t alreadyAttackedZones;

        MonsterZone monsterZones[5];
        SpellTrapZone spellTrapZones[5];
		Card fieldSpell;

        Card hand[80];
        Card deck[80];
        Card grave[95];
        Card banish[95];
        Card extra[15];
    };
    struct Duel
    {
        Player players[2];
    };

    Duel GetDuel()
    {
        Duel duel;
        for (size_t i = 0; i < 2; i++)
        {
            uint32_t address = BASE_PLAYER_ADDRESS + (i * PLAYER_OFFSET);

            Player player;
            player.id = i;

            player.lifePoints = Utils::ReadUint16((void*)(address));
            address += 0x4;
            player.cardsInHand = Utils::ReadUint8((void*)(address));
            address += 0x1;
            player.cardsInDeck = Utils::ReadUint8((void*)(address));
            address += 0x1;
            player.cardsInGrave = Utils::ReadUint8((void*)(address));
            address += 0x1;
            player.cardsInExtra = Utils::ReadUint8((void*)(address));
            address += 0x1;
            player.cardsInBanish = Utils::ReadUint8((void*)(address));
            address += 0x1;
            player.status = Utils::ReadUint8((void*)(address));
            address += 0x1;
            player.canAttackZones = Utils::ReadUint16((void*)(address));
            address += 0x2;
            player.alreadyAttackedZones = Utils::ReadUint16((void*)(address));
            address += 0x2;

            address = BASE_PLAYER_ADDRESS + (i * PLAYER_OFFSET) + 0x10;
            for (size_t j = 0; j < 5; j++)
            {
                Card card;
                card.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
                card.fullValue = Utils::ReadUint32((void*)(address));

                player.monsterZones[j].card = card;
                player.monsterZones[j].effectCount = Utils::ReadUint16((void*)(address + 0xa));
                for (size_t k = 0; k < 32; k++)
                {
                    player.monsterZones[j].effectIDs[k] = Utils::ReadUint16((void*)(address + 0xc + (k * 2)));
                }
                for (size_t k = 0; k < 32; k++)
                {
                    player.monsterZones[j].effectEntries[k].type = Utils::ReadUint8((void*)(address + 0x4c + (k * 2)));
                    player.monsterZones[j].effectEntries[k].value = Utils::ReadUint8((void*)(address + 0x4d + (k * 2)));
                }
                player.monsterZones[j].stateFlags = Utils::ReadUint32((void*)(address + 0x8c));

                address += 0x90;
            }
            for (size_t j = 0; j < 5; j++)
            {
                Card card;
                card.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
                card.fullValue = Utils::ReadUint32((void*)(address));
                player.spellTrapZones[j].card = card;

                address += 0x90;
            }
            Card fieldSpell;
            fieldSpell.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
            fieldSpell.fullValue = Utils::ReadUint32((void*)(address));
            player.fieldSpell = fieldSpell;

            address = BASE_PLAYER_ADDRESS + (i * PLAYER_OFFSET) + 0x6d0;
            for (size_t j = 0; j < 80; j++)
            {
                Card card;
                card.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
                card.fullValue = Utils::ReadUint32((void*)(address));
                player.hand[j] = card;

                address += 0x4;
            }
            for (size_t j = 0; j < 80; j++)
            {
                Card card;
                card.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
                card.fullValue = Utils::ReadUint32((void*)(address));
                player.deck[j] = card;

                address += 0x4;
            }
            for (size_t j = 0; j < 15; j++)
            {
                Card card;
                card.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
                card.fullValue = Utils::ReadUint32((void*)(address));
                player.extra[j] = card;

                address += 0x4;
            }
            for (size_t j = 0; j < 95; j++)
            {
                Card card;
                card.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
                card.fullValue = Utils::ReadUint32((void*)(address));
                player.grave[j] = card;

                address += 0x4;
            }
            for (size_t j = 0; j < 95; j++)
            {
                Card card;
                card.intID = Utils::ReadUint32((void*)(address)) & 0xFFF;
                card.fullValue = Utils::ReadUint32((void*)(address));
                player.banish[j] = card;

                address += 0x4;
            }

            duel.players[i] = player;
        }

        return duel;
    }

    void ChangeSelectionList(std::vector<uint32_t> items)
    {
        Utils::WriteUint8((void*)SELECTION_LIST_SIZE_ADDRESS, items.size());
        for (size_t i = 0; i < items.size(); i++)
        {
            Utils::WriteUint32((void*)(SELECTION_LIST_ADDRESS + (i * 4)), items[i]);
            Utils::WriteUint16((void*)(0x00a584a4 + (i * 2)), 0x4);
        }

    }

    struct EffectScript
    {
        uint32_t CardID;
        uintptr_t Effect;
        uintptr_t AppliesTo;
        uintptr_t Condition;
        uintptr_t Cost;
        uintptr_t Target;
    };

	EffectScript GetEffectScript(int index)
	{
        EffectScript script;

		script.CardID = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript))));
		script.Effect = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 4));
		script.AppliesTo = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 8));
		script.Condition = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 12));
		script.Cost = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 16));
		script.Target = Utils::ReadUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 20));

		return script;
	}
    int GetEffectScriptIndex(uint32_t cardID)
    {
        for (int i = 0; i < 443; i++)
        {
            EffectScript script = GetEffectScript(i);
            if (script.CardID == cardID)
            {
                return i;
            }
        }
        return -1;
    }
	void SetEffectScript(int index, EffectScript script)
	{
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript))), script.CardID);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 4), script.Effect);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 8), script.AppliesTo);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 12), script.Condition);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 16), script.Cost);
		Utils::WriteUint32((void*)(EFFECT_SCRIPT_ADDRESS + (index * sizeof(EffectScript)) + 20), script.Target);
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
	};

	BattleResult GetBattleResult()
	{
		BattleResult result;
		result.StateFlags = Utils::ReadUint32((void*)(0x00a57840));
		for (size_t i = 0; i < 2; i++)
		{
			result.sides[i].ResultFlags = Utils::ReadUint16((void*)(0x00a57844 + (i * 0x10)));
			result.sides[i].IntID = Utils::ReadUint16((void*)(0x00a57846 + (i * 0x10)));
			result.sides[i].ATK = Utils::ReadUint32((void*)(0x00a57848 + (i * 0x10)));
			result.sides[i].DEF = Utils::ReadUint32((void*)(0x00a5784c + (i * 0x10)));
			result.sides[i].DamageTaken = Utils::ReadUint32((void*)(0x00a57850 + (i * 0x10)));
		}
		return result;
	}
}