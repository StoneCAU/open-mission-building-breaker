#pragma once
#include "GameSession.h"
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
    void init();
    void runMenu();
    void runGame();

    void processGameFrame();
    void handleFrameInput();
    void updateFrameState();
    void renderFrame();

    void onGameOver();
    void runGameOver();

    GameState state;
    bool isRunning;
    int highScore;

    GameSession session;
    UIRenderer ui;
};
