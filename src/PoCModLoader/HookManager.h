#pragma once

#include "Utils.h"

using Condition = bool(*)(uint32_t playerIdx);
using Event = void(__stdcall*)();
using Event1 = void(__stdcall*)(uint32_t playerIdx, uint32_t zoneIdx);
using State = uint32_t(__stdcall*)();
using StatChange = void(__stdcall*)(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

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
struct StatChangeHook
{
	uint16_t cardID;
	StatChange statChange;
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
struct InitialSummonStateHook
{
	uint16_t cardIntID;
	uint8_t stateCode;
};
struct SummonStateHook
{
	uint8_t stateCode;
	State state;
};
struct SelectionListPopulationHook
{
	uint16_t cardID;
	Event event;
};

void PatchSpecialSummonCondition();
void PatchPhase();
void PatchStatChange();
void PatchAfterDamageCalculation();
void PatchNormalSummonTrigger();
void PatchSpecialSummonTrigger();
void PatchSpecialSummonTrigger2();
void PatchOnSentToGraveTrigger();
void PatchBanishOnLeavingField();
void PatchInitialSummonState();
void PatchSummonState();
void PatchSelectionListPopulation();

class HookManager
{
public:
	static void InstallHooks();

	static void Register_SpecialSummonCondition(uint16_t id, Condition condition);
	static bool __stdcall Dispatch_SpecialSummonCondition(uint16_t id, uint32_t playerIdx);

	static void Register_Phase(uint32_t phase, Event event);
	static void __stdcall Dispatch_Phase(uint32_t phase);

	static void Register_StatChange(uint16_t id, StatChange statChange);
	static bool __stdcall Dispatch_StatChange(uint16_t id, uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

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

	static void Register_InitialSummonState(uint16_t cardIntID, uint8_t stateCode);
	static uint8_t __stdcall Dispatch_InitialSummonState(uint16_t cardIntID);

	static void Register_SummonState(uint8_t stateCode, State state);
	static uint8_t __stdcall Dispatch_SummonState(uint8_t stateCode);

	static void Register_SelectionListPopulation(uint16_t cardID, Event event);
	static bool __stdcall Dispatch_SelectionListPopulation(uint16_t cardID);

private:
	static inline std::vector<SpecialSummonHook> specialSummonHooks;
	static inline std::vector<PhaseHook> phaseHooks;
	static inline std::vector<StatChangeHook> statChangeHooks;
	static inline std::vector<Event> afterDamageCalculationHooks;
	static inline std::vector<NormalSummonTriggerHook> normalSummonTriggerHooks;
	static inline std::vector<SpecialSummonTriggerHook> specialSummonTriggerHooks;
	static inline std::vector<OnSentToGraveTriggerHook> onSentToGraveTriggerHooks;
	static inline std::vector<BanishOnLeavingFieldHook> banishOnLeavingFieldHooks;
	static inline std::vector<InitialSummonStateHook> initialSummonStateHooks;
	static inline std::vector<SummonStateHook> summonStateHooks;
	static inline std::vector<SelectionListPopulationHook> selectionListPopulationHooks;

	static inline Utils::Hook hSpecialSummonCondition;
	static inline Utils::Hook hPhase;
	static inline Utils::Hook hStatChange;
	static inline Utils::Hook hAfterDamageCalculation;
	static inline Utils::Hook hNormalSummonTrigger;
	static inline Utils::Hook hSpecialSummonTrigger; static inline Utils::Hook hSpecialSummonTrigger2;
	static inline Utils::Hook hOnSentToGraveTrigger;
	static inline Utils::Hook hBanishOnLeavingField;
	static inline Utils::Hook hInitialSummonState;
	static inline Utils::Hook hSummonState;
	static inline Utils::Hook hSelectionListPopulation;
};