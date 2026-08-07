#include "HookManager.h"

namespace
{
	void* gSpecialSummonTrampoline = nullptr;
	void* gPhaseTrampoline = nullptr;
	void* gStatChangeTrampoline = nullptr;
	void* gAfterDamageCalculationTrampoline = nullptr;
	void* gNormalSummonTriggerTrampoline = nullptr;
	void* gSpecialSummonTriggerTrampoline = nullptr; void* gSpecialSummonTriggerTrampoline2 = nullptr;
	void* gOnSentToGraveTriggerTrampoline = nullptr;
	void* gBanishOnLeavingFieldTrampoline = nullptr;
	void* gInitialSummonStateTrampoline = nullptr;
	void* gSummonStateTrampoline = nullptr;
	void* gSelectionListPopulationTrampoline = nullptr;
}

void HookManager::InstallHooks()
{
	hSpecialSummonCondition = Utils::InstallHook((void*)0x005aaeaf, 5, PatchSpecialSummonCondition);
	gSpecialSummonTrampoline = hSpecialSummonCondition.Trampoline;

	hPhase = Utils::InstallHook((void*)0x00404970, 5, PatchPhase);
	gPhaseTrampoline = hPhase.Trampoline;

	hStatChange = Utils::InstallHook((void*)0x0056dde9, 5, PatchStatChange);
	gStatChangeTrampoline = hStatChange.Trampoline;

	hAfterDamageCalculation = Utils::InstallHook((void*)0x00407aae, 5, PatchAfterDamageCalculation);
	gAfterDamageCalculationTrampoline = hAfterDamageCalculation.Trampoline;

	hNormalSummonTrigger = Utils::InstallHook((void*)0x005ac08c, 5, PatchNormalSummonTrigger);
	gNormalSummonTriggerTrampoline = hNormalSummonTrigger.Trampoline;

	hSpecialSummonTrigger = Utils::InstallHook((void*)0x005ad62c, 5, PatchSpecialSummonTrigger);
	gSpecialSummonTriggerTrampoline = hSpecialSummonTrigger.Trampoline;
	hSpecialSummonTrigger2 = Utils::InstallHook((void*)0x005ad13b, 5, PatchSpecialSummonTrigger);
	gSpecialSummonTriggerTrampoline2 = hSpecialSummonTrigger2.Trampoline;

	hOnSentToGraveTrigger = Utils::InstallHook((void*)0x00576f26, 5, PatchOnSentToGraveTrigger);
	gOnSentToGraveTriggerTrampoline = hOnSentToGraveTrigger.Trampoline;

	hBanishOnLeavingField = Utils::InstallHook((void*)0x00576b58, 7, PatchBanishOnLeavingField);
	gBanishOnLeavingFieldTrampoline = hBanishOnLeavingField.Trampoline;

	hInitialSummonState = Utils::InstallHook((void*)0x0059df41, 5, PatchInitialSummonState);
	gInitialSummonStateTrampoline = hInitialSummonState.Trampoline;

	hSummonState = Utils::InstallHook((void*)0x0059deed, 6, PatchSummonState);
	gSummonStateTrampoline = hSummonState.Trampoline;

	hSelectionListPopulation = Utils::InstallHook((void*)0x00599da4, 5, PatchSelectionListPopulation);
	gSelectionListPopulationTrampoline = hSelectionListPopulation.Trampoline;
}
void HookManager::Register_SpecialSummonCondition(uint16_t id, Condition condition)
{
	// Check if the card ID is already registered
	for (const auto& hook : specialSummonHooks)
	{
		if (hook.cardID == id) return;
	}
	specialSummonHooks.push_back({ id, condition });
}
bool __stdcall HookManager::Dispatch_SpecialSummonCondition(uint16_t id, uint32_t playerIdx)
{
	for (const auto& hook : specialSummonHooks)
	{
		if (hook.cardID == id)
		{
			return hook.condition(playerIdx);
		}
	}
	return false;
}
__declspec(naked) void PatchSpecialSummonCondition()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH DWORD PTR DS : [ESP + 8]
		PUSH EAX
		CALL HookManager::Dispatch_SpecialSummonCondition
		TEST AL, AL
		POP EAX
		JZ hook_end

		PUSH 0x005ab148
		RET
	hook_end :
		JMP[gSpecialSummonTrampoline]
	}
}
void HookManager::Register_Phase(uint32_t phase, Event event)
{
	phaseHooks.push_back({ phase, event });
}
void __stdcall HookManager::Dispatch_Phase(uint32_t phase)
{
	for (const auto& hook : phaseHooks)
	{
		if (hook.phase == phase)
		{
			hook.event();
		}
	}
}
__declspec(naked) void PatchPhase()
{
	__asm
	{
	hook:
		PUSH DWORD PTR DS : [ESP + 4]
		CALL HookManager::Dispatch_Phase
	hook_end :
		JMP[gPhaseTrampoline]
	}
}
void HookManager::Register_StatChange(uint16_t id, StatChange statChange)
{
	// Check if the card ID is already registered
	for (const auto& hook : statChangeHooks)
	{
		if (hook.cardID == id) return;
	}
	statChangeHooks.push_back({ id, statChange });
}
bool __stdcall HookManager::Dispatch_StatChange(uint16_t id, uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	for (const auto& hook : statChangeHooks)
	{
		if (hook.cardID == id)
		{
			hook.statChange(statAddress, playerIdx, zoneIdx);
			return true;
		}
	}
	return false;
}
__declspec(naked) void PatchStatChange()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH EDI
		PUSH DWORD PTR DS : [ESP + 68]
		PUSH ESP
		PUSH EAX
		CALL HookManager::Dispatch_StatChange
		TEST AL, AL
		POP EAX
		JZ hook_end

		MOV EAX, 0x0056df89
		JMP EAX
	hook_end :
		JMP[gStatChangeTrampoline]
	}
}
void HookManager::Register_AfterDamageCalculation(Event event)
{
	afterDamageCalculationHooks.push_back(event);
}
void HookManager::Dispatch_AfterDamageCalculation()
{
	for (const auto& hook : afterDamageCalculationHooks)
	{
		hook();
	}
}
__declspec(naked) void PatchAfterDamageCalculation()
{
	__asm
	{
	hook:
		CALL HookManager::Dispatch_AfterDamageCalculation
	hook_end :
		JMP[gAfterDamageCalculationTrampoline]
	}
}
void HookManager::Register_NormalSummonTrigger(uint16_t id)
{
	// Check if the card ID is already registered
	for (const auto& hook : normalSummonTriggerHooks)
	{
		if (hook.cardID == id) return;
	}
	normalSummonTriggerHooks.push_back({ id });
}
bool __stdcall HookManager::Dispatch_NormalSummonTrigger(uint16_t id)
{
	for (const auto& hook : normalSummonTriggerHooks)
	{
		if (hook.cardID == id)
		{
			return true;
		}
	}
	return false;
}
__declspec(naked) void PatchNormalSummonTrigger()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH EAX
		CALL HookManager::Dispatch_NormalSummonTrigger
		TEST AL, AL
		POP EAX
		JZ hook_end

		PUSH 0x005ac110
		RET
	hook_end :
		JMP[gNormalSummonTriggerTrampoline]
	}
}
void HookManager::Register_SpecialSummonTrigger(uint16_t id)
{
	// Check if the card ID is already registered
	for (const auto& hook : specialSummonTriggerHooks)
	{
		if (hook.cardID == id) return;
	}
	specialSummonTriggerHooks.push_back({ id });
}
bool __stdcall HookManager::Dispatch_SpecialSummonTrigger(uint16_t id)
{
	for (const auto& hook : specialSummonTriggerHooks)
	{
		if (hook.cardID == id)
		{
			return true;
		}
	}
	return false;
}
__declspec(naked) void PatchSpecialSummonTrigger()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH EAX
		CALL HookManager::Dispatch_SpecialSummonTrigger
		TEST AL, AL
		POP EAX
		JZ hook_end

		PUSH 0x005ad63a
		RET
	hook_end :
		JMP[gSpecialSummonTriggerTrampoline]
	}
}
__declspec(naked) void PatchSpecialSummonTrigger2()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH EAX
		CALL HookManager::Dispatch_SpecialSummonTrigger
		TEST AL, AL
		POP EAX
		JZ hook_end

		PUSH 0x005ad149
		RET
	hook_end :
		JMP[gSpecialSummonTriggerTrampoline]
	}
}
void HookManager::Register_OnSentToGraveTrigger(uint16_t id, Event1 event)
{
	// Check if the card ID is already registered
	for (const auto& hook : onSentToGraveTriggerHooks)
	{
		if (hook.cardID == id) return;
	}
	onSentToGraveTriggerHooks.push_back({ id, event });
}
bool __stdcall HookManager::Dispatch_OnSentToGraveTrigger(uint16_t id, uint32_t playerIdx, uint32_t zoneIdx)
{
	for (const auto& hook : onSentToGraveTriggerHooks)
	{
		if (hook.cardID == id)
		{
			if (hook.event != nullptr)
			{
				hook.event(playerIdx, zoneIdx);
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}
__declspec(naked) void PatchOnSentToGraveTrigger()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH DWORD PTR DS : [ESP + 0x18]
		PUSH EDI
		PUSH EAX
		CALL HookManager::Dispatch_OnSentToGraveTrigger
		TEST AL, AL
		POP EAX
		JZ hook_end

		PUSH 0x00577080
		RET
	hook_end :
		JMP[gOnSentToGraveTriggerTrampoline]
	}
}
void HookManager::Register_BanishOnLeavingField(uint16_t id)
{
	// Check if the card ID is already registered
	for (const auto& hook : banishOnLeavingFieldHooks)
	{
		if (hook.cardID == id) return;
	}
	banishOnLeavingFieldHooks.push_back({ id });
}
bool __stdcall HookManager::Dispatch_BanishOnLeavingField(uint16_t id)
{
	for (const auto& hook : banishOnLeavingFieldHooks)
	{
		if (hook.cardID == id)
		{
			return true;
		}
	}
	return false;
}
__declspec(naked) void PatchBanishOnLeavingField()
{
	__asm
	{
	hook:
		ADD ESP, 0x4
		PUSH EAX
		AND EAX, 0xFFFF
		PUSH EAX
		CALL HookManager::Dispatch_BanishOnLeavingField
		TEST AL, AL
		POP EAX
		JZ hook_end

		OR EBX, 0x10000
	hook_end :
		SUB ESP, 0x4
		JMP[gBanishOnLeavingFieldTrampoline]
	}
}
void HookManager::Register_InitialSummonState(uint16_t cardIntID, uint8_t state)
{
	// Check if the card ID is already registered
	for (const auto& hook : initialSummonStateHooks)
	{
		if (hook.cardIntID == cardIntID) return;
	}
	initialSummonStateHooks.push_back({ cardIntID, state });
}
uint8_t __stdcall HookManager::Dispatch_InitialSummonState(uint16_t cardIntID)
{
	for (const auto& hook : initialSummonStateHooks)
	{
		if (hook.cardIntID == cardIntID)
		{
			return hook.stateCode;
		}
	}
	return 0;
}
__declspec(naked) void PatchInitialSummonState()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH EAX
		CALL HookManager::Dispatch_InitialSummonState
		TEST AL, AL
		JZ hook_end

		ADD ESP, 0x4
		MOV CL, BYTE PTR DS : [0x00a57808]
		POP EDI
		AND ECX, 0xFF
		POP ESI
		OR CH, AL
		POP EBP
		MOV WORD PTR DS : [0x00a57808] , CX

		PUSH 0x0059e0e1
		RET
	hook_end:
		POP EAX
		JMP[gInitialSummonStateTrampoline]
	}
}
void HookManager::Register_SummonState(uint8_t stateCode, State state)
{
	// Check if the state code is already registered
	for (const auto& hook : summonStateHooks)
	{
		if (hook.stateCode == stateCode) return;
	}
	summonStateHooks.push_back({ stateCode, state });
}
uint8_t __stdcall HookManager::Dispatch_SummonState(uint8_t stateCode)
{
	for (const auto& hook : summonStateHooks)
	{
		if (hook.stateCode == stateCode)
		{
			return hook.state();
		}
	}
	return 2; // Not found
}
__declspec(naked) void PatchSummonState()
{
	__asm
	{
	hook:
		AND ECX, 0xFFFF
		PUSH EBX
		SHR ECX, 0x8
		PUSH EBP
		PUSH ESI
		PUSH ECX
		PUSH EAX
		PUSH ECX
		CALL HookManager::Dispatch_SummonState
		CMP AL, 0x2
		JE hook_end

		ADD ESP, 0x8
		CMP AL, 0x0
		JE hook_0
	hook_1:
		AND WORD PTR DS:[0x00a57804], 0xfffd
		MOV AL, 0x1
		JMP hook_out

	hook_0:
		XOR EAX, EAX

	hook_out:
		POP ESI
		POP EBP
		POP EBX
		ADD ESP, 0x234
		PUSH 0x0059eb1f
		RET
		
	hook_end:
		POP EAX
		POP ECX
		PUSH 0x0059def9
		RET
	}
}
void HookManager::Register_SelectionListPopulation(uint16_t cardID, Event event)
{
	// Check if the card ID is already registered
	for (const auto& hook : selectionListPopulationHooks)
	{
		if (hook.cardID == cardID) return;
	}
	selectionListPopulationHooks.push_back({ cardID, event });
}
bool __stdcall HookManager::Dispatch_SelectionListPopulation(uint16_t cardID)
{
	for (const auto& hook : selectionListPopulationHooks)
	{
		if (hook.cardID == cardID)
		{
			hook.event();
			return true;
		}
	}
	return false;
}
__declspec(naked) void PatchSelectionListPopulation()
{
	__asm
	{
	hook:
		PUSH EBX
		AND EBX, 0xFFFF
		PUSH ECX
		PUSH EDX
		PUSH EAX
		PUSH EBX
		CALL HookManager::Dispatch_SelectionListPopulation
		TEST AL, AL
		POP EAX
		POP EDX
		POP ECX
		POP EBX
		JZ hook_end

		MOV ESI, 0x4
		PUSH 0x0059c432
		RET
	hook_end :
		CMP EBX, 0x460
		JMP[gSelectionListPopulationTrampoline]
	}
}
