#pragma once
#include <SDL2/SDL.h>
#include <string>

class GameSession;
class AssetManager;

class HUDRenderer {
public:
    HUDRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(const GameSession& session);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    void renderScore(int score);
    void renderCombo(int combo);
    void renderSpecialGauge(int gauge);
    void renderLives(int lives);
    void renderNumberImages(const std::string& numberStr, int startX, int y);
};
