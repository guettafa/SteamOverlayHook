#include <dxgi.h>
#include <iostream>
#include "pattern.h"

#pragma warning(disable: 4996)

// CreateHook  : 0x10072980
// Present : 0x10077D20

using CreateHook = char(__cdecl*)(LPCVOID pFuncAddrs, uintptr_t pDetourFuncAddrs, uintptr_t pOriginalPresentFuncAddrs, int a4);
CreateHook CreateHookFunc = nullptr;

using Present = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present PresentOriginal = nullptr;

HRESULT __stdcall MyOwnFunction(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    // My Rendering here
    std::cout << "HELLO FROM MY OWN FUNCTION" << std::endl;

    return PresentOriginal(pSwapChain, SyncInterval, Flags);
}

VOID CreateConsole()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
}

BOOL WINAPI MainThread(HMODULE hModule)
{
    CreateConsole();

    uintptr_t originalPresentFuncAddrs = (uintptr_t)GetModuleHandle(L"dxgi.dll") + (uintptr_t)0x7CC90;
    uintptr_t createHookFuncAddrs  = Pattern::FindPattern(L"GameOverlayRenderer.dll",  Pattern::createHookSig,  "xxxxxxxx");
    uintptr_t hookPresentFuncAddrs = Pattern::FindPattern(L"GameOverlayRenderer.dll",  Pattern::hookPresentSig, "xxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxx");

    std::printf("Original Present func addrs : %x\nCreate Hook Func Addrs : %x\nHook Present Func Addrs : %x\n", originalPresentFuncAddrs, createHookFuncAddrs, hookPresentFuncAddrs);

    PresentOriginal    = (Present)    originalPresentFuncAddrs;
    CreateHookFunc     = (CreateHook) createHookFuncAddrs;

    CreateHookFunc((void*)hookPresentFuncAddrs, (uintptr_t)&MyOwnFunction, (uintptr_t)&originalPresentFuncAddrs, 1);

    std::printf("Own function : %x\n", (uintptr_t)&MyOwnFunction);
    std::cout << "HELLO FROM THREAD !" << std::endl;

    while (true)
    {
        if (GetAsyncKeyState(VK_DOWN))
            break;
    }

    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return FALSE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        
        MessageBox(NULL, L"INJECTED", L"Has been injected", MB_ICONEXCLAMATION);

        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), hModule, 0, nullptr);
    }
    return TRUE;
}

