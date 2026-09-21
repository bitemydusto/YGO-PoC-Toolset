#include "HookAPI.h"
#include "HookManager.h"

extern "C"
{
	void Register_EffectScript(EffectScript script)
	{
		HookManager::Register_EffectScript(script);
	}
	void Register_Fusion2(Fusion2 fusion)
	{
		HookManager::Register_Fusion2(fusion);
	}
	void Register_Fusion3(Fusion3 fusion)
	{
		HookManager::Register_Fusion3(fusion);
	}
	void Register_SpiritMonster(uint16_t cardID)
	{
		HookManager::Register_SpiritMonster(cardID);
	}
	void Register_ExtraSummonMonster(uint16_t cardID, Condition summonCondition, State summonState)
	{
		HookManager::Register_ExtraSummonMonster(cardID, summonCondition, summonState);
	}


	void Register_FlipMonster(uint16_t cardID)
	{
		HookManager::Register_FlipMonster(cardID);
	}
	void Register_ActivatableEffect(uint16_t cardID)
	{
		HookManager::Register_ActivatableEffect(cardID);
	}
	void Register_ActivatableStEffect(uint16_t cardID)
	{
		HookManager::Register_ActivatableStEffect(cardID);
	}
	void Register_InherentSpecialSummon(uint16_t cardID, bool firstOnly)
	{
		HookManager::Register_InherentSpecialSummon(cardID, firstOnly);
	}
	void Register_SpecialSummonCondition(uint16_t id, Condition condition)
	{
		HookManager::Register_SpecialSummonCondition(id, condition);
	}
	void Register_NormalSummonCondition(uint16_t id, Condition condition)
	{
		HookManager::Register_NormalSummonCondition(id, condition);
	}
	void Register_Phase(uint32_t phase, Event event)
	{
		HookManager::Register_Phase(phase, event);
	}
	void Register_StatChange(uint16_t id, StatChange statChange)
	{
		HookManager::Register_StatChange(id, statChange);
	}
	void Register_StatChangeEffect(uint16_t effectId, StatChange statChange)
	{
		HookManager::Regigster_StatChangeEffect(effectId, statChange);
	}
	void Register_StatChangeEquip(uint16_t cardID, StatChange statChange)
	{
		HookManager::Register_StatChangeEquip(cardID, statChange);
	}
	void Register_AfterDamageCalculation(Event event)
	{
		HookManager::Register_AfterDamageCalculation(event);
	}
	void Register_NormalSummonTrigger(uint16_t id)
	{
		HookManager::Register_NormalSummonTrigger(id);
	}
	void Register_CustomNormalSummonTrigger(uint16_t id, Event1 event)
	{
		HookManager::Register_NormalSummonTrigger(id, event);
	}
	void Register_SpecialSummonTrigger(uint16_t id)
	{
		HookManager::Register_SpecialSummonTrigger(id);
	}
	void Register_CustomSpecialSummonTrigger(uint16_t id, Event1 event)
	{
		HookManager::Register_SpecialSummonTrigger(id, event);
	}
	void Register_OnSentToGraveTrigger(uint16_t id, Event1 event)
	{
		HookManager::Register_OnSentToGraveTrigger(id, event);
	}
	void Register_BanishOnLeavingField(uint16_t id)
	{
		HookManager::Register_BanishOnLeavingField(id);
	}
	void Register_InitialSummonState(uint16_t cardID, uint8_t stateCode, bool useDefaultNS)
	{
		HookManager::Register_InitialSummonState(cardID, stateCode, useDefaultNS);
	}
	void Register_SummonState(uint8_t stateCode, State state)
	{
		HookManager::Register_SummonState(stateCode, state);
	}
	void Register_SummonStateByID(uint16_t cardID, State state, bool useDefaultNS)
	{
		HookManager::Register_SummonState(cardID, state, useDefaultNS);
	}
	void Register_SelectionListPopulation(uint16_t cardID, Event event)
	{
		HookManager::Register_SelectionListPopulation(cardID, event);
	}
	void Register_SpellSpeed(uint32_t cardID, uint32_t speed)
	{
		HookManager::Register_SpellSpeed(cardID, speed);
	}
	void Register_HasEffectInHand(uint16_t cardID)
	{
		HookManager::Register_HasEffectInHand(cardID);
	}
	void Register_UnRevivable(uint16_t cardID)
	{
		HookManager::Register_UnRevivable(cardID);
	}
	void Register_CanBeSummonedByEffect(uint16_t cardID, bool canBeSpecialSummoned)
	{
		HookManager::Register_CanBeSummonedByEffect(cardID, canBeSpecialSummoned);
	}
}