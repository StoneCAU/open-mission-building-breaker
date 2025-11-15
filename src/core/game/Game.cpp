#include "Game.h"
#include <windows.h>
#include "GameConfig.h"

#ifdef USE_SDL
    #include "../../platform/sdl/SDLInputHandler.h"
#endif

Game::Game(std::unique_ptr<IRenderer> r, std::unique_ptr<IInputHandler> i)
    : renderer(std::move(r)),
      inputHandler(std::move(i)),
      state(GameState::MENU),
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
    renderer->renderMenu(scoreManager.loadHighScore());
    handleMenuInput();
}

void Game::runGame() {
    session.start();
    renderer->clearScreenFull();
    renderer->renderPlaying(session);

    while (isRunning && state == GameState::PLAYING) {
        processGameFrame();
    }

    if (state == GameState::GAME_OVER) {
        renderer->clearScreen();
    }
}

void Game::runGameOver() {
    displayGameOverScreen();
    handleGameOverInput();
}

void Game::processGameFrame() {
#ifdef USE_SDL
    auto* sdlInput = dynamic_cast<SDLInputHandler*>(inputHandler.get());
    if (sdlInput && !sdlInput->pollEvents()) {
        isRunning = false;
        state = GameState::MENU;
        return;
    }
#endif

    handleFrameInput();
    updateFrameState();
    renderFrame();
    Sleep(GameConfig::FRAME_DELAY_MS);
}

void Game::handleFrameInput() {
    InputKey key = inputHandler->getInput();
    if (key == InputKey::NONE) {
        return;
    }

    session.handleInput(key);
}

void Game::updateFrameState() {
    session.update(inputHandler.get());

    if (session.isGameOver()) {
        onGameOver();
    }
}

void Game::renderFrame() {
    renderer->clearScreen();
    renderer->renderPlaying(session);
}

void Game::handleMenuInput() {
    while (isRunning && state == GameState::MENU) {
#ifdef USE_SDL
        auto* sdlInput = dynamic_cast<SDLInputHandler*>(inputHandler.get());
        if (sdlInput && !sdlInput->pollEvents()) {
            isRunning = false;
            return;
        }
#endif

        InputKey key = inputHandler->getInput();

        if (key == InputKey::NONE) {
            continue;
        }

        if (key == InputKey::ENTER) {
            state = GameState::PLAYING;
            renderer->clearScreen();
            return;
        }

        if (key == InputKey::QUIT) {
            isRunning = false;
            return;
        }
    }
}

void Game::displayGameOverScreen() {
    renderer->clearScreenFull();

    int currentHighScore = scoreManager.loadHighScore();
    GameOverDisplayData data = session.getGameOverData(currentHighScore);

    if (data.isNewRecord) {
        scoreManager.saveHighScore(data.finalScore);
    }

    renderer->renderGameOver(data);
}

void Game::handleGameOverInput() {
    while (isRunning && state == GameState::GAME_OVER) {
#ifdef USE_SDL
        // SDL 이벤트 처리
        auto* sdlInput = dynamic_cast<SDLInputHandler*>(inputHandler.get());
        if (sdlInput && !sdlInput->pollEvents()) {
            isRunning = false;
            return;
        }
#endif

        InputKey key = inputHandler->getInput();

        if (key == InputKey::NONE) {
            continue;
        }

        if (key == InputKey::RESTART) {
            state = GameState::PLAYING;
            renderer->clearScreen();
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
