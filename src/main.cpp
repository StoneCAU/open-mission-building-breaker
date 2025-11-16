#include <iostream>
#include <memory>

#ifdef USE_SDL
    #include <SDL2/SDL.h>
    #include "platform/sdl/SDLRenderer.h"
    #include "platform/sdl/SDLInputHandler.h"
#else
    #include <windows.h>
    #include "platform/console/ConsoleRenderer.h"
    #include "platform/console/ConsoleInputHandler.h"
#endif

#include <windows.h>

#include "core/game/Game.h"

int main(int argc, char* argv[]) {
#ifdef _WIN32
    // 윈도우 콘솔 UTF-8 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale(""));
#endif

#ifdef USE_SDL
    std::cout << "SDL2 버전으로 실행합니다!" << std::endl;

    auto sdlRenderer = std::make_unique<SDLRenderer>();
    if (!sdlRenderer->initialize()) {
        std::cerr << "SDL2 초기화 실패!" << std::endl;
        return 1;
    }

    auto renderer = std::unique_ptr<IRenderer>(std::move(sdlRenderer));
    auto inputHandler = std::make_unique<SDLInputHandler>();

    Game game(std::move(renderer), std::move(inputHandler));
    game.run();

#else
    std::cout << "콘솔 버전으로 실행합니다!" << std::endl;

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    auto renderer = std::make_unique<ConsoleRenderer>();
    auto inputHandler = std::make_unique<ConsoleInputHandler>();

    Game game(std::move(renderer), std::move(inputHandler));
    game.run();
#endif

    return 0;
}
