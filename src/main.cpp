#include <memory>
#include <locale>

#ifdef USE_SDL
    #include <SDL2/SDL.h>
    #include "platform/sdl/base/SDLRenderer.h"
    #include "platform/sdl/base/SDLInputHandler.h"
#else
    #include <windows.h>
    #include "platform/console/ConsoleRenderer.h"
    #include "platform/console/ConsoleInputHandler.h"
#endif

#ifdef _WIN32
    #include <windows.h>
    #include <iostream>
#endif

#include "core/game/Game.h"

namespace {
    constexpr int SUCCESS_EXIT_CODE = 0;
    constexpr int ERROR_EXIT_CODE = 1;

#ifdef _WIN32
    void setupWindowsEnvironment() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        std::locale::global(std::locale(""));
    }

    void hideConsoleCursor() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hConsole, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &cursorInfo);
    }
#endif

#ifdef USE_SDL
    std::unique_ptr<Game> createSDLGame() {
        auto sdlRenderer = std::make_unique<SDLRenderer>();

        const auto initializeRenderer = [&]() {
            auto renderer = std::unique_ptr<IRenderer>(std::move(sdlRenderer));
            auto inputHandler = std::make_unique<SDLInputHandler>();

            return std::make_unique<Game>(std::move(renderer), std::move(inputHandler));
        };

        return sdlRenderer->initialize() ? initializeRenderer() : nullptr;
    }
#else
    std::unique_ptr<Game> createConsoleGame() {
        hideConsoleCursor();

        auto renderer = std::make_unique<ConsoleRenderer>();
        auto inputHandler = std::make_unique<ConsoleInputHandler>();

        return std::make_unique<Game>(std::move(renderer), std::move(inputHandler));
    }
#endif

    std::unique_ptr<Game> createGame() {
#ifdef USE_SDL
        return createSDLGame();
#else
        return createConsoleGame();
#endif
    }

    int runGame() {
        auto game = createGame();

        const auto executeGame = [&]() {
            game->run();
            return SUCCESS_EXIT_CODE;
        };

        int result = ERROR_EXIT_CODE;
        game && (result = executeGame(), true);

        return result;
    }
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    setupWindowsEnvironment();
#endif

    return runGame();
}
