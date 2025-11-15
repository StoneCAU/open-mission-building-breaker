#pragma once
#include "GameSession.h"
#include "ScoreManager.h"
#include "../../ui/UIRenderer.h"

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    GameState state;
    bool isRunning;
    int highScore;

    GameSession session;
    UIRenderer ui;
    ScoreManager scoreManager;

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
