#include "GOAT.h"

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
void Start()
{
	Install_Breaker();
	Install_Tsukuyomi();
    Install_Chaos();
    Install_XYZ();
    Install_Metamorphosis();
    Install_LavaGolem();
    Install_Mirage();
    Install_Tribe();
    Install_Reasoning();
    Install_MonsterGate();
}
