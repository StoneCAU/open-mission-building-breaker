#include "Game.h"
#include "../ui/UIRenderer.h"
#include "../ui/InputHandler.h"
#include <conio.h>
#include <cstdlib>

Game::Game()
    : state(GameState::MENU), isRunning(true), highScore(0) {}

Game::~Game() = default;

void Game::run() {
    UIRenderer ui;
    init();

    ui.renderMenu(highScore);

    while (isRunning) {
        InputKey key = InputHandler::getInput();

        if (key != InputKey::NONE) {
            processInput(key);
            if (state == GameState::MENU) {
                ui.renderMenu(highScore);
            }
        }
    }
}

void Game::init() {
    // TODO: 초기화 로직 (추후 필요 시 추가)
}

void Game::processInput(InputKey key) {
    if (state == GameState::MENU) {
        if (key == InputKey::ENTER) {
            state = GameState::PLAYING;
            system("cls");
        }

        if (key == InputKey::QUIT) {
            isRunning = false;
        }
    }
}

void Game::update() {

}

void Game::render() {
    UIRenderer ui;
    if (state == GameState::MENU) {
        ui.renderMenu(highScore);
    }
}
