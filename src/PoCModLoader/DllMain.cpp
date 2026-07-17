#include <Windows.h>

#include "ModLoader.h"

#pragma comment(linker, "/export:acmFormatSuggest=C:\\Windows\\System32\\msacm32.acmFormatSuggest")
#pragma comment(linker, "/export:acmStreamClose=C:\\Windows\\System32\\msacm32.acmStreamClose")
#pragma comment(linker, "/export:acmStreamConvert=C:\\Windows\\System32\\msacm32.acmStreamConvert")
#pragma comment(linker, "/export:acmStreamOpen=C:\\Windows\\System32\\msacm32.acmStreamOpen")
#pragma comment(linker, "/export:acmStreamPrepareHeader=C:\\Windows\\System32\\msacm32.acmStreamPrepareHeader")
#pragma comment(linker, "/export:acmStreamSize=C:\\Windows\\System32\\msacm32.acmStreamSize")
#pragma comment(linker, "/export:acmStreamUnprepareHeader=C:\\Windows\\System32\\msacm32.acmStreamUnprepareHeader")


DWORD WINAPI LoaderThread(LPVOID lpParam)
{
    //LoadLibraryA("psapi.dll");
    ModLoader loader;
    loader.LoadMods();
    return S_OK;
}
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
        CreateThread(0, 0, &LoaderThread, 0, 0, NULL);
    }

    return TRUE;
}
