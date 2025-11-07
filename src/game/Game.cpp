#include "Game.h"
#include "../ui/UIRenderer.h"
#include "../ui/InputHandler.h"
#include <windows.h>
#include <cstdlib>

namespace {
    void clearScreen() {
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
}

Game::Game() : state(GameState::MENU), isRunning(true), highScore(0) {}
Game::~Game() = default;

void Game::init() {
    // TODO: 초기화 로직 (추후 필요 시 추가)
}

void Game::run() {
    init();

    while (isRunning) {
        if (state == GameState::MENU) {
            handleMenu();
            continue;
        }

        if (state == GameState::PLAYING) {
            startGame();
            continue;
        }

        isRunning = false;
    }
}

void Game::handleMenu() {
    ui.renderMenu(highScore);

    while (isRunning && state == GameState::MENU) {
        InputKey key = InputHandler::getInput();

        if (key == InputKey::NONE)
            continue;

        if (key == InputKey::ENTER) {
            state = GameState::PLAYING;
            clearScreen();
            return;
        }

        if (key == InputKey::QUIT) {
            isRunning = false;
            return;
        }
    }
}

void Game::startGame() {
    session.start();
    clearScreen();
    ui.renderPlaying(session);
    runPlayingLoop();
}

void Game::runPlayingLoop() {
    while (isRunning && state == GameState::PLAYING) {
        InputKey key = InputHandler::getInput();

        if (key != InputKey::NONE)
            session.handleInput(key);

        session.update();

        clearScreen();
        ui.renderPlaying(session);

        if (session.isGameOver()) {
            state = GameState::MENU;
            clearScreen();
            return;
        }

        Sleep(30);
    }
}
