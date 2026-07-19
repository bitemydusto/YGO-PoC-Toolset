#pragma once 

#include <Windows.h>
#include <cstdint>


namespace Utils
{

    struct Hook
    {
        void* Target;
        void* Detour;
        void* Trampoline;

        BYTE Original[16];
        SIZE_T Length;
    };

    Hook InstallHook(void* target, size_t length, void* detour);

    static void WriteBytes(void* address, const void* data, size_t size)
    {
        DWORD old;
        VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old);

        memcpy(address, data, size);

        VirtualProtect(address, size, old, &old);
        FlushInstructionCache(GetCurrentProcess(), address, size);
    }
    static void WriteUint16(void* address, uint16_t value)
    {
        DWORD old;
        VirtualProtect(address, sizeof(value), PAGE_EXECUTE_READWRITE, &old);

        memcpy(address, &value, sizeof(value));

        VirtualProtect(address, sizeof(value), old, &old);
        FlushInstructionCache(GetCurrentProcess(), address, sizeof(value));
    }
    static void WriteJump(void* address, size_t size, void* dst)
    {
        if (size < 5) return;

        DWORD old;
        VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old);

        uintptr_t src_addr = reinterpret_cast<uintptr_t>(address);
        uintptr_t dst_addr = reinterpret_cast<uintptr_t>(dst);
        int32_t relative_offset = static_cast<int32_t>(dst_addr - (src_addr + 5));

        uint8_t jmp_patch[5];
        jmp_patch[0] = 0xE9;

        memcpy(&jmp_patch[1], &relative_offset, sizeof(relative_offset));
        memcpy(address, jmp_patch, sizeof(jmp_patch));

        if (size > 5)
        {
            memset(reinterpret_cast<uint8_t*>(address) + 5, 0x90, size - 5);
        }

        VirtualProtect(address, size, old, &old);
        FlushInstructionCache(GetCurrentProcess(), address, size);
    }
    Hook InstallHook(void* target, size_t length, void* detour)
    {
        Hook hook{};

        hook.Target = target;
        hook.Detour = detour;
        hook.Length = length;

        memcpy(hook.Original, target, length);

        BYTE* trampoline = (BYTE*)VirtualAlloc(nullptr, length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

        if (!trampoline) return hook;
        memcpy(trampoline, hook.Original, length);

        WriteJump(target, length, detour);
        WriteJump(trampoline + length, 5, (BYTE*)target + length);
        hook.Trampoline = trampoline;

        return hook;
    }
}