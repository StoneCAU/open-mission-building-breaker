#include <windows.h>
#include <iostream>
#include <memory>

#include "core/game/Game.h"
#include "platform/console/ConsoleRenderer.h"
#include "platform/console/ConsoleInputHandler.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

#ifdef USE_SDL
    std::cout << "SDL2 version not implemented yet!" << std::endl;
    return 1;
#else
    auto renderer = std::make_unique<ConsoleRenderer>();
    auto inputHandler = std::make_unique<ConsoleInputHandler>();
#endif

    Game game(std::move(renderer), std::move(inputHandler));
    game.run();

    return 0;
}
