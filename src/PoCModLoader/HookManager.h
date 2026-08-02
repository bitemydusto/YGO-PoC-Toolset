#pragma once

#include "Utils.h"

using Condition = bool(*)();

struct SpecialSummonHook
{
	uint16_t cardID;
	Condition condition;
};
void PatchSpecialSummonCondition();

class HookManager
{
public:
	static void InstallHooks();

	static void Register_SpecialSummonCondition(uint16_t id, Condition condition);
	static bool __stdcall Dispatch_SpecialSummonCondition(uint16_t id);

private:
	static inline std::vector<SpecialSummonHook> specialSummonHooks;
	static inline Utils::Hook hSpecialSummonCondition;
};