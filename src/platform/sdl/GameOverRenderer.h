#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../../interfaces/IInputHandler.h"

class AssetManager;
struct GameOverDisplayData;

class GameOverRenderer {
public:
    GameOverRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(const GameOverDisplayData& data);
    void handleInput(InputKey key);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;

    static constexpr int TITLE_Y = 120;
    static constexpr int FINAL_SCORE_Y = 220;
    static constexpr int MAX_COMBO_Y = 250;
    static constexpr int PLAY_TIME_Y = 280;
    static constexpr int NEW_RECORD_Y = 320;
    static constexpr int RESTART_BUTTON_Y = 450;
    static constexpr int QUIT_BUTTON_Y = 480;
    static constexpr int CENTER_X = 400;

    void renderTextCentered(const std::string& text, int x, int y, SDL_Color color);
    std::string buildGameTimeDisplay(int totalSeconds);
};
