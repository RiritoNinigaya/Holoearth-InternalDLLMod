// dllmain.cpp : Defines the entry point for the DLL application.
#include "includes.hpp"
#include <Windows.h>
#include <iostream>
bool init_hook = false;
void Alloc_Console() {
    AllocConsole();
    SetConsoleTitleA("Holoearth Internal Cheat DEBUGGER by RiritoNinigaya");
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    cout << (("CONSOLE IS CREATED"_cs << color(0x3EF52A))) << endl;
}
DWORD WINAPI Thread() {
    Alloc_Console();
    InitUnity_IL2Cpp();
    do
    {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&oPresent, hkPresent);
            init_hook = true;
        }
        else {
            Sleep(8000);
            exit(122);
        }
    } while (!init_hook);
    return TRUE;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

