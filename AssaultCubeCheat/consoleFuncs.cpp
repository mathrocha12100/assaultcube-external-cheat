#include "consoleFuncs.h"

void consoleFuncs::ClearConsole() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

std::string consoleFuncs::DrawCheckbox(bool isChecked) {
    if (isChecked) {
        return " [x]";
    }
    return " [ ]";
}

void consoleFuncs::DrawInConsole(
    bool bHealth,
    bool bAmmo,
    bool bRecoil,
    bool bNoclip
) {
    consoleFuncs::ClearConsole();

    std::cout << "----- Linhas de cmd -----" << std::endl << std::endl;
    std::cout << "[ 0 ] - Sair" << std::endl;
    std::cout << "[ 1 ] - GOD MODE" << DrawCheckbox(bHealth) << std::endl;
    std::cout << "[ 2 ] - Municao Infinita" << DrawCheckbox(bAmmo) << std::endl;
    std::cout << "[ 3 ] - No Recoil" << DrawCheckbox(bRecoil) << std::endl;
    std::cout << "[ 4 ] - Pegar 3 Granadas kk" << std::endl;
    std::cout << "[ 5 ] - Ativar noclip" << DrawCheckbox(bNoclip) << std::endl;
    std::cout << "----- xxxxxxxxxdxxxx -----";
}
