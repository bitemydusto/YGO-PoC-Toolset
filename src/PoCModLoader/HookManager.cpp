#include "HookManager.h"

namespace
{
	void* gSpecialSummonTrampoline = nullptr;
	void* gPhaseTrampoline = nullptr;
	void* gStateChangeTrampoline = nullptr;
}

void HookManager::InstallHooks()
{
	hSpecialSummonCondition = Utils::InstallHook((void*)0x005aaeaf, 5, PatchSpecialSummonCondition);
	gSpecialSummonTrampoline = hSpecialSummonCondition.Trampoline;

	hPhase = Utils::InstallHook((void*)0x00404970, 5, PatchPhase);
	gPhaseTrampoline = hPhase.Trampoline;

	hStateChange = Utils::InstallHook((void*)0x0056dde9, 5, PatchStateChange);
	gStateChangeTrampoline = hStateChange.Trampoline;
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

