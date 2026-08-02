#pragma once

#include <cstdint>


#ifdef MODLOADER_EXPORTS
    #define MOD_API __declspec(dllexport)
#else
    #define MOD_API __declspec(dllimport)
#endif

using Condition = bool(*)();

extern "C"
{
    MOD_API void Register_SpecialSummonCondition(uint16_t id, Condition condition);
    MOD_API void Register_Phase(uint32_t phase, void(__stdcall* event)());
}
