#pragma once
#include "../ui/InputHandler.h"

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
    void processInput(InputKey key);
    void update();
    void render();

    GameState state;
    bool isRunning;
    int highScore;
};
