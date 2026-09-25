#pragma once

#include <cstdint>
#include "Utils.h"


#ifdef MODLOADER_EXPORTS
    #define MOD_API __declspec(dllexport)
#else
    #define MOD_API __declspec(dllimport)
#endif

using Condition = bool(*)(uint32_t playerIdx);
using Condition1 = bool(__stdcall*)(uint8_t side, uint8_t zone);
using Event = void(__stdcall*)();
using Event1 = void(__stdcall*)(uint32_t playerIdx, uint32_t zoneIdx);
using LeavingFieldEvent = bool(__stdcall*)(uint32_t side, uint32_t zone, uint32_t* dest, uint32_t* flags, uint32_t* effectIntID);
using State = uint32_t(__stdcall*)();
using StatChange = void(__stdcall*)(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
using EffectScript = Utils::EffectScript;
using Fusion2 = Utils::Fusion2;
using Fusion3 = Utils::Fusion3;


extern "C"
{
	MOD_API void OncePerTurn(uint8_t side, uint8_t zone);

	MOD_API void Register_EffectScript(EffectScript script);
	MOD_API void Register_Fusion2(Fusion2 fusion);
	MOD_API void Register_Fusion3(Fusion3 fusion);
	MOD_API void Register_SpiritMonster(uint16_t cardID);
	MOD_API void Register_ExtraSummonMonster(uint16_t cardID, Condition summonCondition, State summonState);
	MOD_API void Register_ActivatableGraveEffect(uint16_t cardID);

	MOD_API void Register_FlipMonster(uint16_t cardID);
	MOD_API void Register_ActivatableEffect(uint16_t cardID);
	MOD_API void Register_ActivatableStEffect(uint16_t cardID);
	MOD_API void Register_InherentSpecialSummon(uint16_t cardID, bool firstOnly);
    MOD_API void Register_SpecialSummonCondition(uint16_t id, Condition condition);
	MOD_API void Register_NormalSummonCondition(uint16_t id, Condition condition);
    MOD_API void Register_Phase(uint32_t phase, Event event);
	MOD_API void Register_StatChange(uint16_t id, StatChange statChange);
	MOD_API void Register_StatChangeEffect(uint16_t effectId, StatChange statChange);
	MOD_API void Register_StatChangeEquip(uint16_t cardID, StatChange statChange);
	MOD_API void Register_AfterDamageCalculation(Event event);
    MOD_API void Register_NormalSummonTrigger(uint16_t id);
	MOD_API void Register_CustomNormalSummonTrigger(uint16_t id, Event1 event);
	MOD_API void Register_SpecialSummonTrigger(uint16_t id);
	MOD_API void Register_CustomSpecialSummonTrigger(uint16_t id, Event1 event);
	MOD_API void Register_OnSentToGraveTrigger(uint16_t id, Event1 event);
	MOD_API void Register_BanishOnLeavingField(uint16_t id);
	MOD_API void Register_InitialSummonState(uint16_t cardID, uint8_t stateCode, bool useDefaultNS);
	MOD_API void Register_SummonState(uint8_t stateCode, State state);
	MOD_API void Register_SummonStateByID(uint16_t cardID, State state, bool useDefaultNS);
	MOD_API void Register_SelectionListPopulation(uint16_t cardID, Event event);
	MOD_API void Register_SpellSpeed(uint32_t cardID, uint32_t speed);
	MOD_API void Register_HasEffectInHand(uint16_t cardID);
	MOD_API void Register_UnRevivable(uint16_t cardID);
	MOD_API void Register_CanBeSummonedByEffect(uint16_t cardID, bool canBeSpecialSummoned);
	MOD_API void Register_OnCardLeavingField(LeavingFieldEvent event);
	MOD_API void Register_UnAffectedBySpells(Condition1 condition);
}
