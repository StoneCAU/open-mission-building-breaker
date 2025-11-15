#include "Game.h"
#include <windows.h>

#include "GameConfig.h"
#include "../../ui/InputHandler.h"

Game::Game()
    : state(GameState::MENU),
      isRunning(true),
      highScore(0) {}

Game::~Game() = default;

void Game::run() {
    while (isRunning) {
        if (state == GameState::MENU) {
            runMenu();
        }

        if (state == GameState::PLAYING) {
            runGame();
        }

        if (state == GameState::GAME_OVER) {
            runGameOver();
        }
    }
}

void Game::runMenu() {
    ui.renderMenu(highScore);
    handleMenuInput();
}

void Game::runGame() {
    session.start();
    ui.clearScreenFull();
    ui.renderPlaying(session);

    while (isRunning && state == GameState::PLAYING) {
        processGameFrame();
    }

    if (state == GameState::GAME_OVER) {
        ui.clearScreen();
    }
}

void Game::runGameOver() {
    displayGameOverScreen();
    handleGameOverInput();
}

void Game::processGameFrame() {
    handleFrameInput();
    updateFrameState();
    renderFrame();
    Sleep(GameConfig::FRAME_DELAY_MS);
}

void Game::handleFrameInput() {
    InputKey key = InputHandler::getInput();
    if (key == InputKey::NONE) {
        return;
    }

    session.handleInput(key);
}

void Game::updateFrameState() {
    session.update();

    if (session.isGameOver()) {
        onGameOver();
    }
}

void Game::renderFrame() {
    ui.clearScreen();
    ui.renderPlaying(session);
}

void Game::handleMenuInput() {
    while (isRunning && state == GameState::MENU) {
        InputKey key = InputHandler::getInput();

        if (key == InputKey::NONE) {
            continue;
        }

        if (key == InputKey::ENTER) {
            state = GameState::PLAYING;
            ui.clearScreen();
            return;
        }

        if (key == InputKey::QUIT) {
            isRunning = false;
            return;
        }
    }
}

void Game::displayGameOverScreen() {
    ui.clearScreenFull();

    GameOverDisplayData data = session.getGameOverData(highScore);

    if (data.isNewRecord) {
        highScore = data.finalScore;
    }

    ui.renderGameOver(data);
}

void Game::handleGameOverInput() {
    while (isRunning && state == GameState::GAME_OVER) {
        InputKey key = InputHandler::getInput();

        if (key == InputKey::NONE) {
            continue;
        }

        if (key == InputKey::RESTART) {
            state = GameState::PLAYING;
            ui.clearScreen();
            return;
        }

        if (key == InputKey::QUIT) {
            isRunning = false;
            return;
        }
    }
}

void Game::onGameOver() {
    state = GameState::GAME_OVER;
}
