#include <Windows.h>
#include <cstdio>

#include "HookAPI.h"

// Validation mod for Register_DuelEnd. Appends one line per duel to
// .\mods\duelend.txt so a run can be checked against the x32dbg captures.
//
// Expected, from the two duels already logged:
//   win by reducing the opponent to 0 LP  -> loser=0
//   surrender on your own turn            -> loser=1
// Anything else -- a second line for one duel, loser=2, or nothing at all --
// means the wrapper hook at 0x005bc0c0 needs another look.

void __stdcall OnDuelEnd(uint32_t loserIdx)
{
	const uint8_t phase = *(volatile uint8_t*)0x00a577fa;
	const uint8_t turn = *(volatile uint8_t*)0x00a54e5c & 1;

	SYSTEMTIME t;
	GetLocalTime(&t);

	if (FILE* f = nullptr; fopen_s(&f, ".\mods\duelend.txt", "a") == 0 && f)
	{
		fprintf(f, "%02u:%02u:%02u  loser=%u  phase=0x%02X  result=0x%02X  turn=%u\n",
			t.wHour, t.wMinute, t.wSecond, loserIdx, phase, phase & 0xc0, turn);
		fclose(f);
	}
}

DWORD WINAPI MainThread(LPVOID)
{
	Sleep(500);
	Register_DuelEnd(OnDuelEnd);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinst);
		CreateThread(0, 0, &MainThread, 0, 0, NULL);
	}
	return TRUE;
}
