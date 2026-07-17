#include <Windows.h>

#include "Utils.h"

void GodCards();
void Slifer();

Utils::Hook hTribute_1;
Utils::Hook hTribute_2;
Utils::Hook hSlifer_1;

__declspec(naked) void SliferChangeATK()
{
    __asm
    {
    hook:
        CMP EAX, 0x777
            JNE hook_end
            MOV AL, byte ptr[ESI + 0xa55d68]
            AND EAX, 0xff
            LEA EAX, [EAX + EAX * 0x4]
            LEA EBP, [EAX + EAX * 0x4]
            LEA EBP, [EBP + EBP * 0x4]
            SHL EBP, 0x3
            MOV dword ptr[ESP + 0x18], EBP
            MOV EAX, 0x0056df89
            JMP EAX
            hook_end :
        JMP[hSlifer_1.Trampoline]
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
__declspec(naked) void PerformTripleTribute()
{
    __asm
    {
    hook:
        MOV EAX, 0x004022e0
            CALL EAX
            CMP AX, 0x776
            JL hook_end
            CMP AX, 0x778
            JG hook_end
            MOV EAX, dword ptr[ESP + 0x3c]
            TEST EAX, EAX
            JZ LAB_005b1603
            PUSH 0x0
            PUSH EDI
            PUSH - 0x1
            PUSH ESI
            MOV EAX, 0x005aecb0
            CALL EAX
            MOV word ptr[ESP + 0x48], AX
            MOV ECX, dword ptr[ESP + 0x48]
            PUSH 0x0
            AND ECX, 0xffff
            PUSH EDI
            PUSH ECX
            PUSH ESI
            MOV EAX, 0x005aecb0
            CALL EAX
            MOV word ptr[ESP + 0x48], AX
            MOV ECX, dword ptr[ESP + 0x48]
            PUSH 0x0
            PUSH EDI
            PUSH ECX
            PUSH ESI
            MOV EAX, 0x005aecb0
            CALL EAX
            ADD ESP, 0x20
            CMP AX, 0xffff
            MOV word ptr[ESP + 0x3a], AX
            JZ LAB_005b1603
            MOV EBP, 0x3
            MOV EAX, 0x005b1514
            RET
            LAB_005b1603 :
        MOV EAX, 0x005b1603
            JMP EAX
            hook_end :
        JMP[hTribute_2.Trampoline]
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
    hTribute_2 = Utils::InstallHook((void*)0x005b14cd, 6, (void*)PerformTripleTribute);

    // Effects
    Slifer();
}
void Slifer()
{

    hSlifer_1 = Utils::InstallHook((void*)0x0056e065, 5, (void*)SliferChangeATK);

}
