#include "HookAPI.h"
#include "HookManager.h"

extern "C"
{
	void ReplaceEffectScript(uint32_t oldID, EffectScript script)
	{
		HookManager::ReplaceEffectScript(oldID, script);
	}
	void ReplaceFusion2(uint16_t oldID, Fusion2 fusion)
	{
		HookManager::ReplaceFusion2(oldID, fusion);
	}
	void ReplaceFusion3(uint16_t oldID, Fusion3 fusion)
	{
		HookManager::ReplaceFusion3(oldID, fusion);
	}

	void Register_FlipMonster(uint16_t cardID)
	{
		HookManager::Register_FlipMonster(cardID);
	}
	void Register_ActivatableEffect(uint16_t cardID)
	{
		HookManager::Register_ActivatableEffect(cardID);
	}
	void Register_InherentSpecialSummon(uint16_t cardID)
	{
		HookManager::Register_InherentSpecialSummon(cardID);
	}
	void Register_SpecialSummonCondition(uint16_t id, Condition condition)
	{
		HookManager::Register_SpecialSummonCondition(id, condition);
	}
	void Register_Phase(uint32_t phase, Event event)
	{
		HookManager::Register_Phase(phase, event);
	}
	void Register_StatChange(uint16_t id, StatChange statChange)
	{
		HookManager::Register_StatChange(id, statChange);
	}
	void Register_AfterDamageCalculation(Event event)
	{
		HookManager::Register_AfterDamageCalculation(event);
	}
	void Register_NormalSummonTrigger(uint16_t id)
	{
		HookManager::Register_NormalSummonTrigger(id);
	}
	void Register_SpecialSummonTrigger(uint16_t id)
	{
		HookManager::Register_SpecialSummonTrigger(id);
	}
	void Register_OnSentToGraveTrigger(uint16_t id, Event1 event)
	{
		HookManager::Register_OnSentToGraveTrigger(id, event);
	}
	void Register_BanishOnLeavingField(uint16_t id)
	{
		HookManager::Register_BanishOnLeavingField(id);
	}
	void Register_InitialSummonState(uint16_t cardIntID, uint8_t stateCode)
	{
		HookManager::Register_InitialSummonState(cardIntID, stateCode);
	}
	void Register_SummonState(uint8_t stateCode, State state)
	{
		HookManager::Register_SummonState(stateCode, state);
	}
	void Register_SelectionListPopulation(uint16_t cardID, Event event)
	{
		HookManager::Register_SelectionListPopulation(cardID, event);
	}
	void Register_SpellSpeed(uint32_t cardID, uint32_t speed)
	{
		HookManager::Register_SpellSpeed(cardID, speed);
	}

}