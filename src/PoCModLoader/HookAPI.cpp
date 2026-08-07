#include "HookAPI.h"
#include "HookManager.h"

extern "C"
void Register_SpecialSummonCondition(uint16_t id, Condition condition)
{
    HookManager::Register_SpecialSummonCondition(id, condition);
}
extern "C"
void Register_Phase(uint32_t phase, Event event)
{
	HookManager::Register_Phase(phase, event);
}
extern "C"
void Register_StatChange(uint16_t id, StatChange statChange)
{
	HookManager::Register_StatChange(id, statChange);
}
extern "C"
void Register_AfterDamageCalculation(Event event)
{
	HookManager::Register_AfterDamageCalculation(event);
}
extern "C"
void Register_NormalSummonTrigger(uint16_t id)
{
	HookManager::Register_NormalSummonTrigger(id);
}
extern "C"
void Register_SpecialSummonTrigger(uint16_t id)
{
	HookManager::Register_SpecialSummonTrigger(id);
}
extern "C"
void Register_OnSentToGraveTrigger(uint16_t id, Event1 event)
{
	HookManager::Register_OnSentToGraveTrigger(id, event);
}
extern "C"
void Register_BanishOnLeavingField(uint16_t id)
{
	HookManager::Register_BanishOnLeavingField(id);
}
extern "C"
void Register_InitialSummonState(uint16_t cardIntID, uint8_t stateCode)
{
	HookManager::Register_InitialSummonState(cardIntID, stateCode);
}
extern "C"
void Register_SummonState(uint8_t stateCode, State state)
{
	HookManager::Register_SummonState(stateCode, state);
}
extern "C"
void Register_SelectionListPopulation(uint16_t cardID, Event event)
{
	HookManager::Register_SelectionListPopulation(cardID, event);
}