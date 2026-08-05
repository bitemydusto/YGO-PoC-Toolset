#pragma once

#include <cstdint>


#ifdef MODLOADER_EXPORTS
    #define MOD_API __declspec(dllexport)
#else
    #define MOD_API __declspec(dllimport)
#endif

using Condition = bool(*)();
using Event = void(__stdcall*)();
using Event1 = void(__stdcall*)(uint32_t playerIdx, uint32_t zoneIdx);
using StateChange = void(__stdcall*)(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

extern "C"
{
    MOD_API void Register_SpecialSummonCondition(uint16_t id, Condition condition);
    MOD_API void Register_Phase(uint32_t phase, Event event);
	MOD_API void Register_StateChange(uint16_t id, StateChange stateChange);
	MOD_API void Register_AfterDamageCalculation(Event event);
    MOD_API void Register_NormalSummonTrigger(uint16_t id);
	MOD_API void Register_SpecialSummonTrigger(uint16_t id);
	MOD_API void Register_OnSentToGraveTrigger(uint16_t id, Event1 event);
	MOD_API void Register_BanishOnLeavingField(uint16_t id);
}
