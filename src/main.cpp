#include <windows.h>
#include <iostream>
#include "game/Game.h"

int main() {

    // 콘솔 UTF-8 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Game game;
    game.run();

    return 0;
}
