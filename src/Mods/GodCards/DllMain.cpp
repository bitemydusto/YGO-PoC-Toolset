#include <Windows.h>

#include "Utils.h"

void GodCards();
void Slifer();
void Obelisk();
void Ra();

Utils::Hook hTribute_1;

Utils::Hook hSlifer_1;
Utils::Hook hSlifer_2;
Utils::Hook hSlifer_3;
Utils::Hook hSlifer_4;

Utils::Hook hObelisk_1;

Utils::Hook hRa_1;
Utils::Hook hRa_2;

int combinedRaATK;
int combinedRaDEF;
int tributeATK_1;
int tributeATK_2;
int tributeDEF_1;
int tributeDEF_2;

__declspec(naked) void ATKBuff()
{
    __asm
    {
    hook:
        CMP EAX, 0x778
        JE hook_ra
        CMP EAX, 0x777
        JNE hook_end
    hook_slifer:
        MOV AL, byte ptr[ESI + 0xa55d68]
        AND EAX, 0xff
        LEA EAX, [EAX + EAX * 0x4]
        LEA EBP, [EAX + EAX * 0x4]
        LEA EBP, [EBP + EBP * 0x4]
        SHL EBP, 0x3
        MOV dword ptr[ESP + 0x18], EBP
        MOV EAX, 0x0056df89
        JMP EAX
    hook_ra:
        MOV EAX, combinedRaATK
        MOV dword ptr[ESP + 0x14], EAX
        MOV EAX, combinedRaDEF
        MOV dword ptr[ESP + 0x18], EAX
        MOV EAX, 0x0056df89
        JMP EAX
    hook_end :
        JMP[hSlifer_1.Trampoline]
    }
}
__declspec(naked) void PatchTrapHoleEffect()
{
    __asm
    {
    hook:
		CMP EAX, 0x777
		JNE hook_end
        PUSH ESI
        PUSH EDI
        MOV EAX, 0x0056f5e0
        CALL EAX
        ADD ESP, 0x8
        XOR EDX, EDX
        CMP EAX, 0x7D0
        SETL DL
        MOV EAX, 0x0058d452
        JMP EAX
    hook_end:
		JMP[hSlifer_2.Trampoline]
    }
}
__declspec(naked) void PatchTrapHoleCondition()
{
    __asm
    {
    hook:
        CMP EAX, 0x777
        JNE hook_end
        PUSH EDI
        PUSH ESI
        MOV EAX, 0x0056f5e0
        CAll EAX
        ADD ESP, 0x8
        XOR EDX, EDX
        CMP EAX, 0x7D0
        SETL DL
        POP EDI
        POP ESI
        MOV EAX, EDX
        POP EBX
        PUSH 0x00580538
        RET
    hook_end:
		JMP[hSlifer_3.Trampoline]
    }

}
__declspec(naked) void PatchSpellSPeed()
{
	__asm
	{
	hook:
		CMP EAX, 0x777
		JNE hook_end
        MOV EAX, 0x0057e0a2
        JMP EAX
	hook_end :
		JMP[hSlifer_4.Trampoline]
	}
}

__declspec(naked) void GiveGodsActivatableEffect()
{
    __asm
    {
    hook:
        CMP EAX, 0x776
        JE hook_end
        CMP EAX, 0x777
        JE hook_end
        CMP EAX, 0x778
        JE hook_end
        JMP [hObelisk_1.Trampoline]
    hook_end:
        MOV EAX, 0x0056813b
        JMP EAX
    }
}
__declspec(naked) void AddRaToLPCostPayingFunction()
{
    __asm
    {
    hook:
        CMP EAX, 0x778
        JNE hook_end
        MOV EAX, 0x0057c44b
        JMP EAX
    hook_end :
        JMP[hRa_1.Trampoline]
    }
}
__declspec(naked) void CombineStatsForRa()
{
    __asm
    {
    hook:
        CMP ECX, 0x456
        JNE hook_end
        MOV combinedRaATK, 0x0
        MOV combinedRaDEF, 0x0
        MOV EAX, tributeATK_1
        ADD combinedRaATK, EAX
        MOV EAX, tributeATK_2
        ADD combinedRaATK, EAX
        MOV EAX, tributeDEF_1
        ADD combinedRaDEF, EAX
        MOV EAX, tributeDEF_2
        ADD combinedRaDEF, EAX
    hook_end :
        JMP[hRa_2.Trampoline]
    }
}
__declspec(naked) void ModifiedIsMonsterTributable()
{
    __asm
    {
    hook:
        PUSH ESI
        PUSH EDI
        MOV EDI, DWORD PTR SS : [ESP + 0x10]
        MOV ESI, DWORD PTR SS : [ESP + 0x14]
        MOV EAX, EDI
        AND EAX, 0x1
        LEA EDX, DWORD PTR DS : [ESI + ESI * 0x8]
        SHL EDX, 0x4
        LEA ECX, DWORD PTR DS : [EAX + EAX * 0x2]
        LEA ECX, DWORD PTR DS : [ECX + ECX * 0x8]
        SHL ECX, 0x1
        SUB ECX, EAX
        SHL ECX, 0x4
        ADD ECX, EAX
        MOV AX, WORD PTR DS : [EDX + ECX * 0x4 + 0xa55d74]
        AND EAX, 0xFFF
        PUSH EAX
        MOV AL, BYTE PTR DS : [0x00a57809] // Check state
        CMP AL, 0x3
        JNE hook_second
    hook_first:
        MOV EAX, 0x004027b0
        CALL EAX
        MOV tributeATK_1, EAX // Store atk of first tribute
        MOV EAX, 0x00402800
        CALL EAX
        MOV tributeDEF_1, EAX // Store def of first tribute
        JMP hook_rest
    hook_second:
        MOV EAX, 0x004027b0
        CALL EAX
        MOV tributeATK_2, EAX // Store atk of second tribute
        MOV EAX, 0x00402800
        CALL EAX
        MOV tributeDEF_2, EAX // Store def of second tribute
    hook_rest:
        MOV EAX, 0x004022e0
        CALL EAX
        AND EAX, 0xffff
        ADD ESP, 0x4
        CMP EAX, 781
        JL hook_end
        CMP EAX, 782
        JG hook_end
        POP EDI
        XOR EAX, EAX
        POP ESI
        RET
    hook_end:
        MOV ECX, DWORD PTR SS : [ESP + 0xc]
        PUSH ESI
        PUSH EDI
        PUSH ECX
        MOV EAX, 0x56A140
        CALL EAX
        ADD ESP, 0xc
        POP EDI
        POP ESI
        RET
    }
}
__declspec(naked) void AddTributeRequirement()
{
    __asm
    {
    hook:
        CMP AX, 0x776
        JL hook_end
        CMP AX, 0x778
        JG hook_end
        ADD ESP, 0x4
        PUSH - 0x1
        PUSH ESI
        MOV EAX, 0x0056a200
        CALL EAX
        ADD ESP, 0x8
        CMP EAX, 0x3
        PUSH 0x005aad4f
        RET
    hook_end :
        JMP[hTribute_1.Trampoline]
    }
}
DWORD WINAPI MainThread(LPVOID lpParam)
{
    GodCards();

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

void GodCards()
{
    // Ignore divine checks
    // ATK
    Utils::WriteBytes((void*)0x004027D0, "\xEB\x0B", 2);
    // DEF
    Utils::WriteBytes((void*)0x00402820, "\xEB\x0B", 2);
    // LVL
    Utils::WriteBytes((void*)0x004026e0, "\xEB\x0B", 2);
    // SubType
    Utils::WriteBytes((void*)0x00402758, "\x90\x90", 2);
    Utils::WriteBytes((void*)0x0040275a, "\xEB\x0D", 2);

    // Add 3 tribute requirement
    hTribute_1 = Utils::InstallHook((void*)0x005aac59, 7, (void*)AddTributeRequirement);

    // Effects
    Slifer();
	Obelisk();
    Ra();
}
void Slifer()
{
    hSlifer_1 = Utils::InstallHook((void*)0x0056e065, 5, (void*)ATKBuff);
	hSlifer_2 = Utils::InstallHook((void*)0x0058d3ff, 5, (void*)PatchTrapHoleEffect);
	hSlifer_3 = Utils::InstallHook((void*)0x00580508, 5, (void*)PatchTrapHoleCondition);
	hSlifer_4 = Utils::InstallHook((void*)0x0057e06f, 5, (void*)PatchSpellSPeed);
}
void Obelisk()
{
	hObelisk_1 = Utils::InstallHook((void*)0x00568042, 5, (void*)GiveGodsActivatableEffect);
}
void Ra()
{
	hRa_1 = Utils::InstallHook((void*)0x0057c3d5, 5, (void*)AddRaToLPCostPayingFunction);
	hRa_2 = Utils::InstallHook((void*)0x005ad86d, 8, (void*)CombineStatsForRa);

    Utils::PatchCall(0x0059E485, ModifiedIsMonsterTributable);
    Utils::PatchCall(0x0059E644, ModifiedIsMonsterTributable);
}