#pragma once

#include "Utils.h"

using Condition = bool(*)();
using Event = void(__stdcall*)();
using StateChange = void(__stdcall*)(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

struct SpecialSummonHook
{
	uint16_t cardID;
	Condition condition;
};
struct PhaseHook
{
	uint32_t phase;
	Event event;
};
struct StateChangeHook
{
	uint16_t cardID;
	StateChange stateChange;
};

void PatchSpecialSummonCondition();
void PatchPhase();
void PatchStateChange();

class HookManager
{
public:
	static void InstallHooks();

	static void Register_SpecialSummonCondition(uint16_t id, Condition condition);
	static bool __stdcall Dispatch_SpecialSummonCondition(uint16_t id);

	static void Register_Phase(uint32_t phase, Event event);
	static void __stdcall Dispatch_Phase(uint32_t phase);

	static void Register_StateChange(uint16_t id, StateChange stateChange);
	static bool __stdcall Dispatch_StateChange(uint16_t id, uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

private:
	static inline std::vector<SpecialSummonHook> specialSummonHooks;
	static inline std::vector<PhaseHook> phaseHooks;
	static inline std::vector<StateChangeHook> stateChangeHooks;

	static inline Utils::Hook hSpecialSummonCondition;
	static inline Utils::Hook hPhase;
	static inline Utils::Hook hStateChange;
};