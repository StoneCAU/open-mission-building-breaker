#pragma once

#include <memory>
#include "GameSession.h"
#include "ScoreManager.h"
#include "../../interfaces/IRenderer.h"
#include "../../interfaces/IInputHandler.h"

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
public:
    Game(std::unique_ptr<IRenderer> renderer, std::unique_ptr<IInputHandler> inputHandler);
    ~Game();

    void run();

private:
    GameState state;
    bool isRunning;
    int highScore;

    GameSession session;
    ScoreManager scoreManager;

    std::unique_ptr<IRenderer> renderer;
    std::unique_ptr<IInputHandler> inputHandler;

    void runMenu();
    void runGame();
    void runGameOver();

    void processGameFrame();
    void handleFrameInput();
    void updateFrameState();
    void renderFrame();

    void handleMenuInput();
    void displayGameOverScreen();
    void handleGameOverInput();
    void onGameOver();
};
