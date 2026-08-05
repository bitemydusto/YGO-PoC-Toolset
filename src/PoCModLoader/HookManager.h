#pragma once

#include "Utils.h"

using Condition = bool(*)();
using Event = void(__stdcall*)();
using Event1 = void(__stdcall*)(uint32_t playerIdx, uint32_t zoneIdx);
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
struct NormalSummonTriggerHook
{
	uint16_t cardID;
};
struct SpecialSummonTriggerHook
{
	uint16_t cardID;
};
struct OnSentToGraveTriggerHook
{
	uint16_t cardID;
	Event1 event;
};
struct BanishOnLeavingFieldHook
{
	uint16_t cardID;
};

void PatchSpecialSummonCondition();
void PatchPhase();
void PatchStateChange();
void PatchAfterDamageCalculation();
void PatchNormalSummonTrigger();
void PatchSpecialSummonTrigger();
void PatchSpecialSummonTrigger2();
void PatchOnSentToGraveTrigger();
void PatchBanishOnLeavingField();

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

	static void Register_AfterDamageCalculation(Event event);
	static void __stdcall Dispatch_AfterDamageCalculation();

	static void Register_NormalSummonTrigger(uint16_t id);
	static bool __stdcall Dispatch_NormalSummonTrigger(uint16_t id);

	static void Register_SpecialSummonTrigger(uint16_t id);
	static bool __stdcall Dispatch_SpecialSummonTrigger(uint16_t id);

	static void Register_OnSentToGraveTrigger(uint16_t id, Event1 event);
	static bool __stdcall Dispatch_OnSentToGraveTrigger(uint16_t id, uint32_t playerIdx, uint32_t zoneIdx);

	static void Register_BanishOnLeavingField(uint16_t id);
	static bool __stdcall Dispatch_BanishOnLeavingField(uint16_t id);

private:
	static inline std::vector<SpecialSummonHook> specialSummonHooks;
	static inline std::vector<PhaseHook> phaseHooks;
	static inline std::vector<StateChangeHook> stateChangeHooks;
	static inline std::vector<Event> afterDamageCalculationHooks;
	static inline std::vector<NormalSummonTriggerHook> normalSummonTriggerHooks;
	static inline std::vector<SpecialSummonTriggerHook> specialSummonTriggerHooks;
	static inline std::vector<OnSentToGraveTriggerHook> onSentToGraveTriggerHooks;
	static inline std::vector<BanishOnLeavingFieldHook> banishOnLeavingFieldHooks;

	static inline Utils::Hook hSpecialSummonCondition;
	static inline Utils::Hook hPhase;
	static inline Utils::Hook hStateChange;
	static inline Utils::Hook hAfterDamageCalculation;
	static inline Utils::Hook hNormalSummonTrigger;
	static inline Utils::Hook hSpecialSummonTrigger; static inline Utils::Hook hSpecialSummonTrigger2;
	static inline Utils::Hook hOnSentToGraveTrigger;
	static inline Utils::Hook hBanishOnLeavingField;
};