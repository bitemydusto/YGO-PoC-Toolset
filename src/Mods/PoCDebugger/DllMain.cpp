#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>

using Fn0 = uint32_t(__cdecl*)();
using Fn1 = uint32_t(__cdecl*)(uint32_t);
using Fn2 = uint32_t(__cdecl*)(uint32_t, uint32_t);
using Fn3 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t);
using Fn4 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t);
using Fn5 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
using Fn6 = uint32_t(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

void Start();

uint32_t CallFunction(uintptr_t address, const std::vector<uint32_t>& args);

DWORD WINAPI MainThread(LPVOID lpParam)
{
    Sleep(500);
    Start();

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
uint32_t CallFunction(uintptr_t address, const std::vector<uint32_t>& args)
{
    switch (args.size())
    {
        case 0:
        {
            auto fn = reinterpret_cast<Fn0>(address);
            return fn();
        }
        case 1:
        {
            auto fn = reinterpret_cast<Fn1>(address);
            return fn(args[0]);
        }
        case 2:
        {
            auto fn = reinterpret_cast<Fn2>(address);
            return fn(args[0], args[1]);
        }
        case 3:
        {
            auto fn = reinterpret_cast<Fn3>(address);
            return fn(args[0], args[1], args[2]);
        }
        case 4:
        {
            auto fn = reinterpret_cast<Fn4>(address);
            return fn(args[0], args[1], args[2], args[3]);
        }
        case 5:
        {
            auto fn = reinterpret_cast<Fn5>(address);
            return fn(args[0], args[1], args[2], args[3], args[4]);
        }
        case 6:
        {
            auto fn = reinterpret_cast<Fn6>(address);
            return fn(args[0], args[1], args[2], args[3], args[4], args[5]);
        }
    }
}
void Start()
{
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);

    std::cout << "Debugger ready.\n";

    std::string line;

    while (std::getline(std::cin, line))
    {
        std::stringstream ss(line);

        std::string command;
        ss >> command;

        if (command.empty())
        {
            continue;
        }

        if (command == "help")
        {
            std::cout << "call <address> [arg0] [arg1] ... [arg5]\n";
            continue;
        }

        if (command != "call")
        {
            std::cout << "Unknown command. Type 'help' for usage.\n";
            continue;
        }

        std::string addressString;

        if (!(ss >> addressString))
        {
            std::cout << "Usage: call <address> [arg0] [arg1] ... [arg5]\n";
            continue;
        }
        uintptr_t address = std::stoull(addressString, nullptr, 0);
        std::vector<uint32_t> args;
        std::string argString;

        while (ss >> argString)
        {

            uint32_t value = static_cast<uint32_t>(std::stoull(argString, nullptr, 0));

            args.push_back(value);
        }

        uint32_t result = CallFunction(address, args);
        std::cout << "Return value: 0x"
            << std::hex << result
            << std::dec
            << " (" << result << ")\n";
    }
}