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
void Register_StateChange(uint16_t id, StateChange stateChange)
{
	HookManager::Register_StateChange(id, stateChange);
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