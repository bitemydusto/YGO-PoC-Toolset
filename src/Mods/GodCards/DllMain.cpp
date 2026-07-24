#include <Windows.h>

#include "Utils.h"

void GodCards();
void Slifer();
void Ra();

Utils::Hook hTribute_1;
Utils::Hook hTribute_2;
Utils::Hook hTribute_3;
Utils::Hook hTribute_4;

Utils::Hook hSlifer_1;
Utils::Hook hSlifer_2;
Utils::Hook hSlifer_3;
Utils::Hook hSlifer_4;

Utils::Hook hActEffects_1;

Utils::Hook hRa_1;
Utils::Hook hRa_2;
Utils::Hook hRa_3;

int combinedRaATK;
int combinedRaDEF;
int tributeATK_1;
int tributeATK_2;
int tributeATK_3;
int tributeDEF_1;
int tributeDEF_2;
int tributeDEF_3;

uint16_t monsterID;

__declspec(naked) void StatBuff()
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
        JMP [hActEffects_1.Trampoline]
    hook_end:
        MOV EAX, 0x0056813b
        JMP EAX
    }
}
__declspec(naked) void PatchLPCostPayingFunction()
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
// On normal summon
__declspec(naked) void CombineStatsForRa()
{
    __asm
    {
    hook:
        CMP ECX, 0x456 // IntID
        JNE hook_end
        MOV combinedRaATK, 0x0
        MOV combinedRaDEF, 0x0

        MOV EAX, tributeATK_1
        ADD combinedRaATK, EAX
        MOV EAX, tributeATK_2
        ADD combinedRaATK, EAX
        MOV EAX, tributeATK_3
        ADD combinedRaATK, EAX

        MOV EAX, tributeDEF_1
        ADD combinedRaDEF, EAX
        MOV EAX, tributeDEF_2
        ADD combinedRaDEF, EAX
        MOV EAX, tributeDEF_3
        ADD combinedRaDEF, EAX
    hook_end :
        JMP[hRa_2.Trampoline]
    }
}
__declspec(naked) void ResetRaStatsOnSpecialSummon()
{
    __asm
    {
    hook:
        AND ECX, 0x0FFF
        CMP ECX, 0x456 // IntID
        JNE hook_end
        CMP BYTE PTR  DS : [0x00a5508c] , 0x24 // Check if the summon type is special summon
        JNE hook_end
        MOV combinedRaATK, 0x0
        MOV combinedRaDEF, 0x0
    hook_end:
        JMP[hRa_3.Trampoline]
    }
}
// Save the the ID of the monster about to be tribute summoned (lvl 7 or higher)
__declspec(naked) void SaveMonsterID()
{
    __asm
    {
    hook:
        MOV monsterID, AX
        CMP AX, 0x776
        JL hook_2
        CMP AX, 0x778
        JG hook_2
        PUSH ECX
        MOV ECX, DWORD PTR DS : [0x005f24c0]
        MOV BYTE PTR DS : [ECX + 0x76], 0x33
        POP ECX
        JMP hook_end
    hook_2:
        PUSH ECX
        MOV ECX, DWORD PTR DS : [0x005f24c0]
        MOV BYTE PTR DS : [ECX + 0x76], 0x32
        POP ECX
    hook_end:
        JMP[hTribute_3.Trampoline]
    }
}
// Save the stats of the tributes when you hover over them
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
        CMP AL, 0x4
        JE hook_second
        CMP AL, 0x5
        JE hook_third
    hook_first: // State 0x3
        MOV EAX, 0x004027b0
        CALL EAX
        MOV tributeATK_1, EAX // Store atk of first tribute
        MOV EAX, 0x00402800
        CALL EAX
        MOV tributeDEF_1, EAX // Store def of first tribute
        JMP hook_rest
    hook_second: // State 0x4
        MOV EAX, 0x004027b0
        CALL EAX
        MOV tributeATK_2, EAX // Store atk of second tribute
        MOV EAX, 0x00402800
        CALL EAX
        MOV tributeDEF_2, EAX // Store def of second tribute
        JMP hook_rest
    hook_third: // State 0x5
        MOV EAX, 0x004027b0
        CALL EAX
        MOV tributeATK_3, EAX // Store atk of third tribute
        MOV EAX, 0x00402800
        CALL EAX
        MOV tributeDEF_3, EAX // Store def of third tribute
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
// Modfies unused state 4 so it works like state 3 for god cards
__declspec(naked) void AdditionalTributeState()
{
    __asm
    {
    hook:
        PUSH EAX
        MOV AX, monsterID
        CMP AX, 0x776
        JL hook_end
        CMP AX, 0x778
        JG hook_end
        POP EAX

        TEST DWORD PTR DS : [0x00a54e54] , 0x102
        JZ LAB_1
        MOV EAX, 0x00486c30
        CALL EAX
        MOV DL, BYTE PTR DS : [0x00a57808]
        POP EDI
        AND EDX, 0xFF
        POP ESI
        OR DH, 0x1
        POP EBP
        MOV WORD PTR DS : [0x00a57808] , DX
        XOR EAX, EAX
        POP EBX
        ADD ESP, 0x234
        RET
    LAB_1 :
        PUSH 0xF000F0
        MOV EAX, 0x005aa410
        CALL EAX
        ADD ESP, 0x4
        TEST EAX, EAX
        JNZ LAB_8
        MOV EAX, 0x0059eb13
        JMP EAX 
    LAB_8:
        // Check against state 3
        MOV EAX, DWORD PTR DS : [0x00a55080]
        MOV ECX, DWORD PTR DS : [0x00a55044]
        MOV EDX, EAX
        SHR EDX, 0x1C
        AND EDX, 0x1
        CMP ECX, EDX
        MOV EDX, DWORD PTR DS : [0x00a5504c]
        JNZ LAB_2
        SHR EAX, 0x10
        AND EAX, 0x7
        CMP EDX, EAX
        JNZ LAB_2
        MOV EAX, 0x0059eb13
        JMP EAX
    LAB_2 :
        MOV DX, WORD PTR DS : [0x00a5780a]
        MOV EAX, DWORD PTR DS : [0x00a5504c]
        MOV ECX, DWORD PTR DS : [0x00a55044]
        PUSH EAX
        SHR EDX, 0x8
        AND EDX, 0x1
        PUSH ECX
        PUSH EDX
        MOV EAX, OFFSET ModifiedIsMonsterTributable
        CALL EAX
        ADD ESP, 0xC
        TEST EAX, EAX
        JNZ LAB_3
        MOV EAX, DWORD PTR SS : [ESP + 0x24c]
        TEST EAX, EAX
        JNZ LAB_4
        MOV EAX, 0x0059eb13
        JMP EAX
    LAB_4 :
        MOV DX, WORD PTR DS : [0x00a5780a]
        MOV EAX, DWORD PTR DS : [0x00a5504c]
        MOV ECX, DWORD PTR DS : [0x00a55044]
        PUSH EAX
        SHR EDX, 0x8
        AND EDX, 0x1
        PUSH ECX
        PUSH EDX
        MOV EAX, 0x0056a140
        CALL EAX
        ADD ESP, 0xC
        TEST EAX, EAX
        JNZ LAB_3
        MOV EAX, 0x0059eb13
        JMP EAX
    LAB_3 :
        MOV AX, WORD PTR DS : [0x00a5780a]
        SHR EAX, 0x8
        AND EAX, 0x1
        PUSH EAX
        MOV EAX, 0x0056a000
        CALL EAX
        ADD ESP, 0x4
        TEST EAX, EAX
        JNZ LAB_5
        MOV CX, WORD PTR DS : [0x00a5780a]
        MOV EAX, DWORD PTR DS : [0x00a55044]
        SHR ECX, 0x8
        AND ECX, 0x1
        CMP EAX, ECX
        JZ LAB_6
        MOV EAX, 0x0059eb13
        JMP EAX
    LAB_6 :
        MOV EDX, DWORD PTR DS : [0x00a5504c]
        PUSH EDX
        PUSH EAX
        MOV EAX, 0x00569e10
        CALL EAX
        ADD ESP, 0x8
        TEST EAX, EAX
        JNZ LAB_5
        MOV EAX, 0x0059eb13
        JMP EAX
    LAB_5 :
        MOV EAX, 0x005aa450
        CALL EAX
        TEST EAX, EAX
        JNZ LAB_7
        MOV EAX, 0x0059eb13
        JMP EAX
    LAB_7 :
        PUSH 0x1
        MOV EAX, 0x0044f250
        CALL EAX
        MOV EAX, DWORD PTR DS : [0x00a55048]
        MOV ECX, DWORD PTR DS : [0x00a5504c]
        MOV EDX, DWORD PTR DS : [0x00a55044]
        ADD EAX, ECX
        PUSH EAX
        PUSH EDX
        MOV EAX, 0x00486bb0
        CALL EAX
        MOV EAX, DWORD PTR DS : [0x00a55044]
        MOV ECX, DWORD PTR DS : [0x00a5504c]
        MOV EDX, DWORD PTR DS : [0x00a55080]
        // Bit packing for summon paramaters, using the unused descriptor (0x8000000)
        AND EAX, 0x1
        SHL EAX, 0x1E

        AND ECX, 0x7
        SHL ECX, 0x16

        OR  ECX, EAX
        OR  ECX, 0x08000000

        AND EDX, 0xB63FFFFF
        ADD ESP, 0xC
        OR  ECX, EDX

        MOV DWORD PTR DS : [0x00a55080] , ECX
        MOV EAX, DWORD PTR DS : [0x00a57808]
        MOV CL, BYTE PTR DS : [0x00a57808]
        AND EAX, 0xFF00
        AND ECX, 0xFF
        ADD EAX, 0x100
        POP EDI
        XOR EAX, ECX
        POP ESI
        MOV WORD PTR DS : [0x00a57808] , AX
        POP EBP
        XOR EAX, EAX
        POP EBX
        ADD ESP, 0x234

        PUSH 0x0059e5a5
        RET

    hook_end:
        POP EAX
		JMP[hTribute_2.Trampoline]
    }
}
// Extra check for state 5
__declspec(naked) void State5CheckAgainstState4()
{
    __asm
    {
    hook:
        MOV EAX, DWORD PTR DS: [0x00a55080]
        MOV ECX, DWORD PTR DS: [0x00a55044]
        MOV EDX, EAX
        SHR EDX, 0x1e
        AND EDX, 0x1
        CMP ECX, EDX
        MOV EDX, DWORD PTR DS: [0x00a5504c]
        JNZ hook_end
        MOV ECX, EAX
        SHR ECX, 0x16
        AND ECX, 0x7
        CMP EDX, ECX
        JNZ hook_end
        MOV EAX, 0x0059eb13
        JMP EAX
    hook_end:
		JMP[hTribute_4.Trampoline]
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

	// Implement triple tribute summoning
    hTribute_1 = Utils::InstallHook((void*)0x005aac59, 7, (void*)AddTributeRequirement);
	hTribute_2 = Utils::InstallHook((void*)0x0059e5a6, 6, (void*)AdditionalTributeState);
	hTribute_3 = Utils::InstallHook((void*)0x0059df39, 5, (void*)SaveMonsterID);
	hTribute_4 = Utils::InstallHook((void*)0x0059e635, 6, (void*)State5CheckAgainstState4);

    Utils::PatchCall(0x0059E485, ModifiedIsMonsterTributable);
    Utils::PatchCall(0x0059E644, ModifiedIsMonsterTributable);

    // Effects
    hActEffects_1 = Utils::InstallHook((void*)0x00568042, 5, (void*)GiveGodsActivatableEffect);

    Slifer();
    Ra();
}
void Slifer()
{
    hSlifer_1 = Utils::InstallHook((void*)0x0056e065, 5, (void*)StatBuff);
	hSlifer_2 = Utils::InstallHook((void*)0x0058d3ff, 5, (void*)PatchTrapHoleEffect);
	hSlifer_3 = Utils::InstallHook((void*)0x00580508, 5, (void*)PatchTrapHoleCondition);
	hSlifer_4 = Utils::InstallHook((void*)0x0057e06f, 5, (void*)PatchSpellSPeed);
}
void Ra()
{
	hRa_1 = Utils::InstallHook((void*)0x0057c3d5, 5, (void*)PatchLPCostPayingFunction);
	hRa_2 = Utils::InstallHook((void*)0x005ad86d, 8, (void*)CombineStatsForRa);
	hRa_3 = Utils::InstallHook((void*)0x005AD890, 6, (void*)ResetRaStatsOnSpecialSummon);
}