#pragma once

#include "Utils.h"

using Condition = bool(*)();
using Event = void(__stdcall*)();

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

void PatchSpecialSummonCondition();
void PatchPhase();

class HookManager
{
public:
	static void InstallHooks();

	static void Register_SpecialSummonCondition(uint16_t id, Condition condition);
	static bool __stdcall Dispatch_SpecialSummonCondition(uint16_t id);

	static void Register_Phase(uint32_t phase, Event event);
	static void __stdcall Dispatch_Phase(uint32_t phase);

private:
	static inline std::vector<SpecialSummonHook> specialSummonHooks;
	static inline std::vector<PhaseHook> phaseHooks;

	static inline Utils::Hook hSpecialSummonCondition;
	static inline Utils::Hook hPhase;
};