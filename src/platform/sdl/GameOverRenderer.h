#pragma once

#include <SDL2/SDL.h>
#include "AssetManager.h"

struct GameOverDisplayData;

class GameOverRenderer {
public:
    GameOverRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(const GameOverDisplayData& data);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    void renderTextCentered(const std::string& text, int x, int y, SDL_Color color);
    std::string buildGameTimeDisplay(int totalSeconds);
};
