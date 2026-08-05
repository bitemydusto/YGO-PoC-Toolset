#include "HookManager.h"

namespace
{
	void* gSpecialSummonTrampoline = nullptr;
	void* gPhaseTrampoline = nullptr;
	void* gStateChangeTrampoline = nullptr;
	void* gAfterDamageCalculationTrampoline = nullptr;
	void* gNormalSummonTriggerTrampoline = nullptr;
	void* gSpecialSummonTriggerTrampoline = nullptr; void* gSpecialSummonTriggerTrampoline2 = nullptr;
	void* gOnSentToGraveTriggerTrampoline = nullptr;
	void* gBanishOnLeavingFieldTrampoline = nullptr;
}

void HookManager::InstallHooks()
{
	hSpecialSummonCondition = Utils::InstallHook((void*)0x005aaeaf, 5, PatchSpecialSummonCondition);
	gSpecialSummonTrampoline = hSpecialSummonCondition.Trampoline;

	hPhase = Utils::InstallHook((void*)0x00404970, 5, PatchPhase);
	gPhaseTrampoline = hPhase.Trampoline;

	hStateChange = Utils::InstallHook((void*)0x0056dde9, 5, PatchStateChange);
	gStateChangeTrampoline = hStateChange.Trampoline;

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
bool __stdcall HookManager::Dispatch_SpecialSummonCondition(uint16_t id)
{
	for (const auto& hook : specialSummonHooks)
	{
		if (hook.cardID == id)
		{
			return hook.condition();
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
void HookManager::Register_StateChange(uint16_t id, StateChange stateChange)
{
	// Check if the card ID is already registered
	for (const auto& hook : stateChangeHooks)
	{
		if (hook.cardID == id) return;
	}
	stateChangeHooks.push_back({ id, stateChange });
}
bool __stdcall HookManager::Dispatch_StateChange(uint16_t id, uint32_t statAddress, uint32_t playerIdx, uint32_t zoneIdx)
{
	for (const auto& hook : stateChangeHooks)
	{
		if (hook.cardID == id)
		{
			hook.stateChange(statAddress, playerIdx, zoneIdx);
			return true;
		}
	}
	return false;
}
__declspec(naked) void PatchStateChange()
{
	__asm
	{
	hook:
		PUSH EAX
		PUSH EDI
		PUSH DWORD PTR DS : [ESP + 68]
		PUSH ESP
		PUSH EAX
		CALL HookManager::Dispatch_StateChange
		TEST AL, AL
		POP EAX
		JZ hook_end

		MOV EAX, 0x0056df89
		JMP EAX
	hook_end :
		JMP[gStateChangeTrampoline]
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
