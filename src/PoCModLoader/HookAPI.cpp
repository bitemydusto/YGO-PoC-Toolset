#include "HookAPI.h"
#include "HookManager.h"

extern "C"
void Register_SpecialSummonCondition(uint16_t id, Condition condition)
{
    HookManager::Register_SpecialSummonCondition(id, condition);
}
extern "C"
void Register_Phase(uint32_t phase, void(__stdcall* event)())
{
	HookManager::Register_Phase(phase, event);
}