#pragma once

#include "Utils.h"
#include "PatchLoader.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
#include "FUN.h"

namespace GameData
{
	struct Card;
}

using Condition = bool(*)(uint32_t playerIdx);
using Condition1 = bool(__stdcall*)(uint8_t side, uint8_t zone);
using ScriptFUN = uint32_t(__cdecl*)(unsigned int* param, int param2, int param3);
using Event = void(__stdcall*)();
using Event1 = void(__stdcall*)(uint32_t playerIdx, uint32_t zoneIdx);
using Event2 = void(__stdcall*)(uint32_t playerIdx);
using LeavingFieldEvent = bool(__stdcall*)(uint32_t side, uint32_t zone, uint32_t* dest, uint32_t* action, uint32_t* effectIntID);
using LosingBattleEvent = void(__stdcall*)(uint32_t sideIdx, uint32_t zoneIdx, uint32_t* zone);
using EffectActivatedEvent = void(__stdcall*)(unsigned int* srcParam, uint8_t respondingSide);
using State = uint32_t(__stdcall*)();
using StatChange = void(__stdcall*)(uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);
using EffectScript = Utils::EffectScript;
using SpellSpeedHook = Utils::SpellSpeed;
using Fusion2 = Utils::Fusion2;
using Fusion3 = Utils::Fusion3;

struct SpecialSummonHook
{
	uint16_t cardID;
	Condition condition;
};
struct PhaseHook
{
	uint32_t phase;
	Event event;
	bool resolved;
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
struct NormalSummonCustomTriggerHook
{
	uint16_t cardID;
	Event1 event;
};
struct SpecialSummonTriggerHook
{
	uint16_t cardID;
};
struct SpecialSummonCustomTriggerHook
{
	uint16_t cardID;
	Event1 event;
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
	uint16_t cardID;
	uint8_t stateCode;
	bool useDefaultNS;
};
struct SummonStateHook
{
	uint8_t stateCode;
	State state;
};
struct SummonStateHook2
{
	uint16_t cardID;
	State state;
	bool useDefaultNS;
};
struct SelectionListPopulationHook
{
	uint16_t cardID;
	Event2 event;
};
struct CanBeSpecialSummonedByEffectHook
{
	uint16_t cardID;
	bool canBeSpecialSummoned;
};
struct ExtraMonster
{
	uint16_t cardID;
	Condition summonCondition;
	State summonState;
};
struct CorrectResponseWindows
{
	uint16_t cardIntID;
	vector<uint8_t> windows;
};

void PatchCardEffectScript1();
void PatchCardEffectScript2();
void PatchCardEffectScript3();
void PatchCardEffectScript4();
void PatchCardEffectScript5();
void PatchCardEffectScript6();
void PatchCardEffectScript7();
void PatchCardEffectScript8();
void PatchCardEffectScript9();
void PatchCardEffectScript10();
void PatchCardEffectScript11();

void PatchSpecialSummonCondition();
void PatchNormalSummonCondition();
void PatchPhase();
void PatchStatChange();
void PatchStatCHange2();
void PatchStatChange3();
void PatchAfterDamageCalculation();
void PatchNormalSummonTrigger();
void PatchSpecialSummonTrigger();
void PatchSpecialSummonTrigger2();
void PatchOnSentToGraveTrigger();
void PatchOnSentToGraveTrigger2();
void PatchBanishOnLeavingField();
void PatchInitialSummonState();
void PatchSummonState();
void PatchSelectionListPopulation();
void PatchInherentSpecialSummon();
void PatchActivatableEffect();
void PatchActivatableStEffect();
void PatchFlipMonster();
void PatchSpellSpeed();
void PatchHasEffectInHand();
void PatchHasEffectInHand2();
void PatchCanBeRevived();
void PatchCanBeTargeted();
void PatchCanBeSpecialSummonedByEffect();
void PatchCardHover();
void PatchInputProcess();
void PatchCardLeavingField();
void PatchUnAffectedBySpell();
void PatchOnEffectActivated();
void PatchLimitStatus();
void PatchSummonMonster();
void PatchZoneOverlay();
void PatchLosingBattle();
void PatchCorrectResponseWindow();
void PatchCanSideStartChain();
void PatchChainCardFromHand();

void __stdcall LoadSelectionListExtra(uint32_t playerIdx);
void __stdcall LoadSelectionListGrave(uint32_t playerIdx);

void __stdcall ResetOncePerTurnFlags();
void __stdcall ReturnSpiritsToHand();

class HookManager
{
public:
	static void InstallHooks();
	static void PatchCards();
	static void LoadTags();

	static void SetOncePerTurnFlag(uint8_t side, uint8_t zone);
	static bool UsedEffectThisTurn(uint8_t side, uint8_t zone);
	static void SetHardOncePerTurnFlag(uint16_t cardID, uint8_t side);
	static bool CardWasUsedThisTurn(uint16_t cardID, uint8_t side);

	static void AddTag(uint16_t cardID, uint16_t tag);
	static bool HasTag(uint16_t cardID, uint16_t tag);

	static void AddSpellCounter(uint8_t side, uint8_t zone);
	static void RemoveSpellCounter(uint8_t side, uint8_t zone);
	static uint8_t GetSpellCounters(uint8_t side, uint8_t zone);

	static void Register_EffectScript(EffectScript script);
	static void Register_Fusion2(Fusion2 fusion);
	static void Register_Fusion3(Fusion3 fusion);
	static void Register_SpiritMonster(uint16_t cardID);
	static void Register_TunerMonster(uint16_t cardID);
	static void Register_SynchroMonster(uint16_t cardID);
	static bool IsSpiritMonster(uint16_t cardID);
	static bool IsSynchroMonster(uint16_t cardID);
	static bool IsTunerMonster(uint16_t cardID);
	static void Register_ExtraSummonMonster(uint16_t cardID, Condition summonCondition, State summonState);

	static void Register_FlipMonster(uint16_t cardID);
	static bool __stdcall Dispatch_FlipMonster(uint16_t cardID);

	static void Register_ActivatableEffect(uint16_t cardID);
	static bool __stdcall Dispatch_ActivatableEffect(uint16_t cardID);

	static void Register_ActivatableStEffect(uint16_t cardIntID);
	static bool __stdcall Dispatch_ActivatableStEffect(uint16_t cardIntID, uint8_t zoneIdx);

	static void Register_InherentSpecialSummon(uint16_t cardID, bool firstOnly);
	static bool __stdcall Dispatch_InherentSpecialSummon(uint16_t cardID);

	static void Register_SpecialSummonCondition(uint16_t id, Condition condition);
	static bool __stdcall Dispatch_SpecialSummonCondition(uint16_t id, uint32_t playerIdx);

	static void Register_NormalSummonCondition(uint16_t id, Condition condition);
	static uint32_t __stdcall Dispatch_NormalSummonCondition(uint16_t id, uint32_t playerIdx);

	static void Register_Phase(uint32_t phase, Event event);
	static void __stdcall Dispatch_Phase(uint32_t phase);

	static void Register_StatChange(uint16_t id, StatChange statChange);
	static bool __stdcall Dispatch_StatChange(uint16_t id, uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

	static void Regigster_StatChangeEffect(uint16_t effectId, StatChange statChange);
	static bool __stdcall Dispatch_StatChangeEffect(uint16_t effectId, uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

	static void Register_StatChangeEquip(uint16_t cardID, StatChange statChange);
	static bool __stdcall Dispatch_StatChangeEquip(uint16_t cardID, uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx);

	static void Register_AfterDamageCalculation(Event event);
	static void __stdcall Dispatch_AfterDamageCalculation();

	static void Register_NormalSummonTrigger(uint16_t id);
	static void Register_NormalSummonTrigger(uint16_t id, Event1 event);
	static bool __stdcall Dispatch_NormalSummonTrigger(uint16_t id, uint32_t summonParam);

	static void Register_SpecialSummonTrigger(uint16_t id);
	static void Register_SpecialSummonTrigger(uint16_t id, Event1 event);
	static bool __stdcall Dispatch_SpecialSummonTrigger(uint16_t id, uint32_t summonParam);

	static void Register_OnSentToGraveTrigger(uint16_t id, Event1 event);
	static bool __stdcall Dispatch_OnSentToGraveTrigger(uint16_t id, uint32_t playerIdx, uint32_t zoneIdx);

	static void Register_BanishOnLeavingField(uint16_t id);
	static bool __stdcall Dispatch_BanishOnLeavingField(uint16_t id);

	static void Register_InitialSummonState(uint16_t cardID, uint8_t stateCode, bool useDefaultNS);
	static uint8_t __stdcall Dispatch_InitialSummonState(uint16_t cardID, uint32_t summonType);

	static void Register_SummonState(uint8_t stateCode, State state);
	static void Register_SummonState(uint16_t cardID, State state, bool useDefaultNS);
	static uint8_t __stdcall Dispatch_SummonState(uint8_t stateCode);

	static void Register_SelectionListPopulation(uint16_t cardID, Event2 event);
	static bool __stdcall Dispatch_SelectionListPopulation(uint16_t cardID, uint32_t playerIdx);

	static void Register_SpellSpeed(uint32_t cardID, uint32_t speed);
	static uint32_t __stdcall Dispatch_SpellSpeed(uint32_t cardID);

	static void Register_HasEffectInHand(uint16_t cardID);
	static bool __stdcall Dispatch_HasEffectInHand(uint16_t cardID);

	static void Register_UnRevivable(uint16_t cardID);
	static bool __stdcall Dispatch_UnRevivable(uint16_t cardIntID);

	static void Register_UnTargetable(uint16_t cardID);
	static bool __stdcall Dispatch_UnTargetable(uint8_t side, uint8_t zone);

	static void Register_CanBeSummonedByEffect(uint16_t cardID, bool canBeSpecialSummoned);
	static uint32_t __stdcall Dispatch_CanBeSummonedByEffect(uint16_t cardID);

	static void Register_OnCardLeavingField(LeavingFieldEvent event);
	static bool __stdcall Dispatch_OnCardLeavingField(uint32_t side, uint32_t zone, uint32_t* dest, uint32_t* action, uint32_t* effectIntID);

	static void Register_UnAffectedBySpells(Condition1 condition);
	static bool __stdcall Dispatch_UnAffectedBySpells(uint8_t side, uint8_t zone);

	static void Register_ActivatableGraveEffect(uint16_t cardIntID);
	static uint32_t __stdcall Dispatch_ActivatableGraveEffect();

	static void Register_OnEffectActivated(EffectActivatedEvent event);
	static bool __stdcall Dispatch_OnEffectActivated(unsigned int* srcParam, uint8_t respondingSide);

	static void SetLimitStatus(uint16_t cardID, uint16_t limit);
	static uint16_t __stdcall Dispatch_LimitStatus(uint16_t cardIntID);

	static void Register_ResponseWindow(uint16_t cardID, uint8_t responseWindow);
	static bool __stdcall Dispatch_ResponseWindow(uint8_t side, uint16_t cardIntID, uint8_t responseWindow);

	static void Register_MonsterLosingBattle(LosingBattleEvent event);
	static bool __stdcall Dispatch_MonsterLosingBattle(uint32_t sideIdx, uint32_t zoneIdx, uint32_t* zone, uint32_t param4, uint32_t param5);

	static void Register_OnMonsterSummon(Event event);
	static void __stdcall Dispatch_OnMonsterSummon();

	static void __stdcall Dispatch_ZoneOverlay(int actor, int renderer);
	static bool __stdcall Dispatch_CanSideStartChain(uint16_t cardIntID, uint8_t side);
	static bool __stdcall Dispatch_ChainCardFromHand(uint32_t* cards);

	static inline std::unordered_map<CardTag, std::vector<uint16_t>> cardTags;

	static inline std::vector<uint16_t> spiritMonsters;
	static inline std::vector<uint16_t> tunerMonsters;
	static inline std::vector<uint16_t> synchroMonsters;
	static inline std::vector<PhaseHook> phaseHooks;
	static inline std::vector<ExtraMonster> extraMonsters;
	static inline std::vector<uint16_t> activatableGraveEffects;
	static inline std::vector<uint16_t> hardOPTs[2];

	static bool __stdcall Dispatch_CardHover();
	static inline uint32_t __stdcall ExtraSummonState();

	static inline uint16_t selectedExtraMonster;
	static inline GameData::Card* selectedGraveCard = nullptr;
	static inline int innerExtraSummonState;
	static inline int extraRunning;
	static inline int graveRunning;

	static void ReadZoneOverlaySprites();

private:
	static int __cdecl M_GetEffectScriptIndex(uint32_t cardID);
	static uint32_t __cdecl M_GetNumOfFusionReqs(uint32_t cardIntID);
	static int __cdecl M_GetFusionMaterial(uint32_t cardIntID, uint32_t materialIndex);
	static int  __cdecl M_CanFuse(uint32_t player, uint32_t fusionIntId, uint16_t* out);
	static uint32_t __cdecl M_CanSideRespond(unsigned int*, unsigned int respondingSide);

	static inline EffectScript effectScripts[4096];
	static inline Fusion2 fusionRecipes2[4096];
	static inline Fusion3 fusionRecipes3[4096];
	static inline uint16_t cardLimits[4096] = {3};

	static inline std::vector<std::string> zoneOverlaySprites;


	static inline std::vector<uint16_t> flipMonsters;
	static inline std::vector<uint16_t> activatableEffects;
	static inline std::vector<uint16_t> activatableStEffects;
	static inline std::vector<uint16_t> inherentSpecialSummons;
	static inline std::vector<SpecialSummonHook> specialSummonHooks;
	static inline std::vector<SpecialSummonHook> normalSummonHooks;
	static inline std::vector<StatChangeHook> statChangeHooks;
	static inline std::vector<StatChangeHook> statChangeHooks2;
	static inline std::vector<StatChangeHook> statChangeHooks3;
	static inline std::vector<Event> afterDamageCalculationHooks;
	static inline std::vector<NormalSummonTriggerHook> normalSummonTriggerHooks;
	static inline std::vector<NormalSummonCustomTriggerHook> normalSummonCustomTriggerHooks;
	static inline std::vector<SpecialSummonTriggerHook> specialSummonTriggerHooks;
	static inline std::vector<SpecialSummonCustomTriggerHook> specialSummonCustomTriggerHooks;
	static inline std::vector<OnSentToGraveTriggerHook> onSentToGraveTriggerHooks;
	static inline std::vector<BanishOnLeavingFieldHook> banishOnLeavingFieldHooks;
	static inline std::vector<InitialSummonStateHook> initialSummonStateHooks;
	static inline std::vector<SummonStateHook> summonStateHooks;
	static inline std::vector<SummonStateHook2> summonStateHooks2;
	static inline std::vector<SelectionListPopulationHook> selectionListPopulationHooks;
	static inline std::vector<SpellSpeedHook> spellSpeedHooks;
	static inline std::vector<uint16_t> hasEffectInHandHooks;
	static inline std::vector<uint16_t> unRevivableHooks;
	static inline std::vector<uint16_t> unTargetableHooks;
	static inline std::vector<CanBeSpecialSummonedByEffectHook> canBeSpecialSummonedByEffectHooks;
	static inline std::vector<LeavingFieldEvent> onCardLeavingFieldHooks;
	static inline std::vector<Condition1> unAffectedBySpellHooks;
	static inline std::vector<EffectActivatedEvent> onEffectActivatedHooks;
	static inline std::vector<Event> onMonsterSummonHooks;
	static inline std::vector<LosingBattleEvent> monsterLosingBattleHooks;
	static inline std::vector<CorrectResponseWindows> responseWindows;

	static inline Utils::Hook hCardEffectSctript1;
	static inline Utils::Hook hCardEffectSctript2;
	static inline Utils::Hook hCardEffectSctript3;
	static inline Utils::Hook hCardEffectSctript4;
	static inline Utils::Hook hCardEffectSctript5;
	static inline Utils::Hook hCardEffectSctript6;
	static inline Utils::Hook hCardEffectSctript7;
	static inline Utils::Hook hCardEffectSctript8;
	static inline Utils::Hook hCardEffectSctript9;
	static inline Utils::Hook hcardEffectScript10;
	static inline Utils::Hook hcardEffectScript11;

	static inline Utils::Hook hFlipMonster;
	static inline Utils::Hook hActivatableEffect;
	static inline Utils::Hook hActivatableStEffect;
	static inline Utils::Hook hInherentSpecialSummon;
	static inline Utils::Hook hSpecialSummonCondition;
	static inline Utils::Hook hNormalSummonCondition;
	static inline Utils::Hook hPhase;
	static inline Utils::Hook hStatChange;
	static inline Utils::Hook hStatChange2;
	static inline Utils::Hook hStatChange3;
	static inline Utils::Hook hAfterDamageCalculation;
	static inline Utils::Hook hNormalSummonTrigger;
	static inline Utils::Hook hSpecialSummonTrigger; static inline Utils::Hook hSpecialSummonTrigger2;
	static inline Utils::Hook hOnSentToGraveTrigger; static inline Utils::Hook hOnSentToGraveTrigger2;
	static inline Utils::Hook hBanishOnLeavingField;
	static inline Utils::Hook hInitialSummonState;
	static inline Utils::Hook hSummonState;
	static inline Utils::Hook hSelectionListPopulation;
	static inline Utils::Hook hSpellSpeed;
	static inline Utils::Hook hHasEffectInHand;
	static inline Utils::Hook hHasEffectInHand2;
	static inline Utils::Hook hCanBeRevived;
	static inline Utils::Hook hCanBeTargeted;
	static inline Utils::Hook hCanBeSpecialSummonedByEffect;
	static inline Utils::Hook hListClicked;
	static inline Utils::Hook hCardHover;
	static inline Utils::Hook hCardHover2;
	static inline Utils::Hook hCardHover3;
	static inline Utils::Hook hOnCardLeavingField;
	static inline Utils::Hook hUnAffectedBySpell;
	static inline Utils::Hook hOnEffectActivated;
	static inline Utils::Hook hLimitStatus;
	static inline Utils::Hook hSummonMonster;
	static inline Utils::Hook hZoneOverlay;
	static inline Utils::Hook hLosingBattle;
	static inline Utils::Hook hCorrectResponseWindow;
	static inline Utils::Hook hCanSideStartChain;
	static inline Utils::Hook hChainCardFromHand;


	// Library

	static void InstallLibraryHooks();

	static inline uint16_t cardCollection[4096];
	static uint16_t __stdcall M_GetCardIntID(uint16_t cardID);
	static uint16_t __stdcall M_GetCardID(uint16_t cardID);
	static void __stdcall M_LoadAlbum(uint32_t album);
	static bool __stdcall M_RebuildDeck();

	static void PatchGetCardIntID();
	static void PatchGetCardID();

	static void PatchRebuildDecks();
	static void PatchGrantCopy();
	static void PatchGrantUntil();
	//static void PatchCanAddToDeck();
	static void PatchRebuildAlbum();
	static void PatchDeckBuilder();

	static void PatchLoadAlbum();
	static void PatchRebuildDeck();
	static void PatchLoadDeckBuilder();

	static void PatchLoadDeckBuilder1();
	static void PatchLoadDeckBuilder2();
	static void PatchLoadDeckBuilder3();
	static void PatchLoadDeckBuilder4();
	static void PatchValidateMainDeck();
	static void PatchValidateSideDeck();
	static void PatchValidateExtraDeck();
	static void PatchClearCollectionFlags();
	static void PatchCanAddToDeck();

	static void PatchRebuildDeck1();

	static void PatchFUN_004673e0();
};