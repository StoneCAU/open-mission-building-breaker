#include "Game.h"
#include <windows.h>
#include "GameConfig.h"

#ifdef USE_SDL
    #include "../../platform/sdl/base/SDLInputHandler.h"
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
        (state == GameState::MENU) && (runMenu(), true);
        (state == GameState::PLAYING) && (runGame(), true);
        (state == GameState::GAME_OVER) && (runGameOver(), true);
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
    if (!pollSDLEvents()) return;

    handleFrameInput();
    updateFrameState();
    renderFrame();
    Sleep(GameConfig::FRAME_DELAY_MS);
}

void Game::handleFrameInput() {
    InputKey key = inputHandler->getInput();
    if (key != InputKey::NONE) {
        session.handleInput(key);
    }
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
        if (!pollSDLEvents()) return;

        InputKey key = inputHandler->getInput();
        if (key == InputKey::NONE) continue;

        if (processMenuEvent(key)) return;
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
        if (!pollSDLEvents()) return;

        InputKey key = inputHandler->getInput();
        if (key == InputKey::NONE) continue;

        if (processGameOverEvent(key)) return;
    }
}

void Game::onGameOver() {
    state = GameState::GAME_OVER;
}


bool Game::pollSDLEvents() {
#ifdef USE_SDL
    auto* sdlInput = dynamic_cast<SDLInputHandler*>(inputHandler.get());
    if (sdlInput && !sdlInput->pollEvents()) {
        isRunning = false;
        return false;
    }
#endif
    return true;
}

bool Game::processMenuEvent(InputKey key) {
    renderer->handleMenuInput(key);

    if (key == InputKey::ENTER) {
        state = GameState::PLAYING;
        renderer->clearScreen();
        return true;
    }

    if (key == InputKey::QUIT) {
        isRunning = false;
        return true;
    }

    return false;
}

bool Game::processGameOverEvent(InputKey key) {
    renderer->handleGameOverInput(key);

    if (key == InputKey::RESTART) {
        state = GameState::PLAYING;
        renderer->clearScreen();
        return true;
    }

    if (key == InputKey::QUIT) {
        isRunning = false;
        return true;
    }

    return false;
}
