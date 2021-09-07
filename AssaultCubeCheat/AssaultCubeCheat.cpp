#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include "mem.h"

/*
*  Primeiro cheat externo que desenvolvi kkk
*  mathrocha 06/09/2021
*/

int main() {
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAdrr = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false;

    const int newValue = 1337;

    DWORD procId = GetProcId(L"ac_client.exe");

    if (!procId) {
        std::cout << "Process not found, press enter to exit\n";
        getchar();
        return 0;
    }

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    moduleBase = GetModuleBaseAdress(procId, L"ac_client.exe");

    localPlayerPtr = moduleBase + 0x10f4f4; 
    uintptr_t granadePtr = moduleBase + 0x10FC84;

    healthAdrr = FindDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
    
    uintptr_t axisXAddr = FindDMAAddy(hProcess, moduleBase + 0x109B74, { 0x34 });
    uintptr_t granadeVisualAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0x158 });
    uintptr_t granadeRealAddr = FindDMAAddy(hProcess, granadePtr, { 0x0, 0x2DC });

    DWORD dwExit = 0;
    
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {

        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            bAmmo = !bAmmo;

            if (bAmmo) {
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else {
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
            bRecoil = !bRecoil;

            if (bRecoil) {
                mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
            }
            else {
                mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
            }
        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
            int granade = 2;
         
            mem::PatchEx((BYTE*)granadeVisualAddr, (BYTE*)&granade, sizeof(granade), hProcess);
            mem::PatchEx((BYTE*)granadeRealAddr, (BYTE*)&granade, sizeof(granade), hProcess);
        }

        if (GetAsyncKeyState(VK_NUMPAD5) & 1) {
            float newPosition = 76.0f;

            mem::PatchEx((BYTE*)axisXAddr, (BYTE*)&newPosition, sizeof(newPosition), hProcess);
        }

        if (GetAsyncKeyState(VK_INSERT) & 1) {
            return 0;
        }

        if (bHealth) {
            mem::PatchEx((BYTE*)healthAdrr, (BYTE*)&newValue, sizeof(newValue), hProcess);
        }
        Sleep(10);
    }

    std::cout << "Process not found, press enter to exit";
    getchar();
    return 0;
}