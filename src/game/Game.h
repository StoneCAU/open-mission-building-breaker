#pragma once

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
    void processInput();
    void update();
    void render();

    GameState state;
    bool isRunning;
};
