#include "Game.h"
#include <windows.h>

#include "GameConfig.h"
#include "../../ui/InputHandler.h"

Game::Game()
    : state(GameState::MENU),
      isRunning(true),
      highScore(0) {}

Game::~Game() = default;

void Game::init() {
    // TODO: 초기화 로직 (필요 시 추가)
}

void Game::run() {
    init();

    while (isRunning) {
        if (state == GameState::MENU) {
            runMenu();
            continue;
        }

        if (state == GameState::PLAYING) {
            runGame();
            continue;
        }

        if (state == GameState::GAME_OVER) {
            runGameOver();
            continue;
        }


        isRunning = false;
    }
}

/** ===================== [MENU LOOP] ===================== **/
void Game::runMenu() {
    ui.renderMenu(highScore);

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

/** ===================== [GAME LOOP] ===================== **/
void Game::runGame() {
    session.start();
    ui.clearScreen();
    ui.renderPlaying(session);

    while (isRunning && state == GameState::PLAYING) {
        processGameFrame();
    }

    if (state == GameState::GAME_OVER) {
        ui.clearScreen();
    }
}

/** ===================== [FRAME CYCLE] ===================== **/
void Game::processGameFrame() {
    handleFrameInput();
    updateFrameState();
    renderFrame();
    Sleep(GameConfig::FRAME_DELAY_MS);
}

/** ===== 입력 처리 ===== **/
void Game::handleFrameInput() {
    InputKey key = InputHandler::getInput();
    if (key == InputKey::NONE) {
        return;
    }

    session.handleInput(key);
}

/** ===== 상태 갱신 ===== **/
void Game::updateFrameState() {
    session.update();

    if (session.isGameOver()) {
        onGameOver();
    }
}

/** ===== 렌더링 ===== **/
void Game::renderFrame() {
    ui.clearScreen();
    ui.renderPlaying(session);
}

/** ===== 게임 종료 처리 ===== **/
void Game::onGameOver() {
    state = GameState::GAME_OVER;
}

void Game::runGameOver() {
    ui.clearScreenFull();

    int finalScore = session.getScore();
    int maxCombo = session.getMaxCombo();
    bool isNewRecord = finalScore > highScore;

    if (isNewRecord) {
        highScore = finalScore;
    }

    ui.renderGameOver(finalScore, maxCombo, highScore, isNewRecord);

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
