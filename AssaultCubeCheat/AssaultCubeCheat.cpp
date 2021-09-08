#include <iostream>
#include <vector>
#include <Windows.h>

#include "proc.h"
#include "mem.h"
#include "consoleFuncs.h"

/*
*  Primeiro cheat externo que desenvolvi kkk
*  mathrocha 06/09/2021
*/

int main() {
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0, localPlayerPtr = 0, visualHealthAdrr = 0;
    bool bHealth = false, bAmmo = false, bRecoil = false, bNoclip = false;

    const int newValue = 999999999;

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

    visualHealthAdrr = FindDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
    uintptr_t healthAddrReal = FindDMAAddy(hProcess, moduleBase + 0x10FC84, { 0x0, 0x27C });

    uintptr_t axisXAddr = FindDMAAddy(hProcess, moduleBase + 0x109B74, { 0x34 });

    uintptr_t granadeVisualAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0x158 });
    uintptr_t granadeRealAddr = FindDMAAddy(hProcess, granadePtr, { 0x0, 0x2DC });

    DWORD dwExit = 0;

   

    consoleFuncs::DrawInConsole(bHealth, bAmmo, bRecoil, bNoclip);
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {

        if (GetAsyncKeyState(VK_NUMPAD5)) {
            bNoclip = !bNoclip;
            consoleFuncs::DrawInConsole(bHealth, bAmmo, bRecoil, bNoclip);
        }

        if (bNoclip && GetAsyncKeyState(VK_UP) & 1) {
            DWORD value = mem::GetValueOfAdress(axisXAddr, hProcess, true);
            float newPositionInXAxis = value + 2.0f;

            mem::PatchEx((BYTE*)axisXAddr, (BYTE*)&newPositionInXAxis, sizeof(newPositionInXAxis), hProcess);
        }

        if (bNoclip && GetAsyncKeyState(VK_DOWN) & 1) {
            DWORD value = mem::GetValueOfAdress(axisXAddr, hProcess, true);
            float newPositionInXAxis = value - 2.0f;

            mem::PatchEx((BYTE*)axisXAddr, (BYTE*)&newPositionInXAxis, sizeof(newPositionInXAxis), hProcess);
        }
        
        if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
            bHealth = !bHealth;
            consoleFuncs::DrawInConsole(bHealth, bAmmo, bRecoil, bNoclip);
            
            if (bHealth) {
                mem::NopEx((BYTE*)(moduleBase + 0x29D1F), 1, hProcess);
            }
            else {
                mem::PatchEx((BYTE*)(moduleBase + 0x29D1F), (BYTE*)"\x29\x7B\x04", 1, hProcess);
            }
          
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
            bAmmo = !bAmmo;
            consoleFuncs::DrawInConsole(bHealth, bAmmo, bRecoil, bNoclip);

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
            consoleFuncs::DrawInConsole(bHealth, bAmmo, bRecoil, bNoclip);

        }

        if (GetAsyncKeyState(VK_NUMPAD4) & 1) {
            int granade = 3;
         
            mem::PatchEx((BYTE*)granadeVisualAddr, (BYTE*)&granade, sizeof(granade), hProcess);
            mem::PatchEx((BYTE*)granadeRealAddr, (BYTE*)&granade, sizeof(granade), hProcess);
            consoleFuncs::DrawInConsole(bHealth, bAmmo, bRecoil, bNoclip);
        }

        if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
            return 0;
        }

       
        Sleep(10);
    }

    std::cout << "Process not found, press enter to exit";
    getchar();
    return 0;
}