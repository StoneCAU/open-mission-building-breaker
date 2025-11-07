#pragma once
#include "GameSession.h"
#include "../ui/UIRenderer.h"

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
    // ====== 초기화 및 루프 ======
    void init();
    void runMenu();
    void runGame();

    // ====== 프레임 단위 처리 ======
    void processGameFrame();
    void handleFrameInput();
    void updateFrameState();
    void renderFrame();

    // ====== 게임 상태 전환 ======
    void onGameOver();

private:
    GameState state;
    bool isRunning;
    int highScore;

    GameSession session;
    UIRenderer ui;
};
