#pragma once

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

namespace consoleFuncs {
    void ClearConsole();
    std::string DrawCheckbox(bool isChecked);
    void DrawInConsole(
        bool bHealth,
        bool bAmmo,
        bool bRecoil,
        bool bNoclip
    );
}

