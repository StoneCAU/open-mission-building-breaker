#include "Game.h"
#include "../ui/UIRenderer.h"
#include "../ui/InputHandler.h"
#include <conio.h>
#include <cstdlib>

Game::Game(): state(GameState::MENU), isRunning(true), highScore(0) {}
Game::~Game() = default;

void Game::init() {
    // TODO: 초기화 로직 (추후 필요 시 추가)
}

void Game::run() {
    init();

    while (isRunning) {
        if (state == GameState::MENU) {
            runMenu(); continue;
        }

        if (state == GameState::PLAYING) {
            runPlaying(); continue;
        }

        isRunning = false;
    }
}

void Game::runMenu() {
    ui.renderMenu(highScore);

    while (isRunning && state == GameState::MENU) {
        InputKey key = InputHandler::getInput();

        if (key == InputKey::NONE) continue;

        if (key == InputKey::ENTER) {
            state = GameState::PLAYING;
            system("cls");
            return;
        }

        if (key == InputKey::QUIT) {
            isRunning = false;
            return;
        }
    }
}

void Game::runPlaying() {
    // TODO: 게임 플레이 화면
}

