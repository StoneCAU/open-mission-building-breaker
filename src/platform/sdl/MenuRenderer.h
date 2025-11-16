#pragma once

#include <SDL2/SDL.h>
#include "AssetManager.h"
#include <string>

class MenuRenderer {
public:
    MenuRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(int highScore);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    void renderBackground();
    void renderTitle();
    void renderScoreBox(int highScore);
    void renderButtons();
    void renderTextCentered(const std::string& text, int x, int y, const std::string& fontName, SDL_Color color);
    void renderRect(int x, int y, int w, int h, SDL_Color color);
};
