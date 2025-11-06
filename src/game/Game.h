#pragma once
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
    void init();
    void handleMenu();
    void startGame();
    void initPlaying();

    GameState state;
    bool isRunning;
    int highScore;

    UIRenderer ui;
};
