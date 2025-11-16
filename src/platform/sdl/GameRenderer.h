#pragma once

#include <SDL2/SDL.h>
#include "AssetManager.h"
#include "PixelSpriteRenderer.h"
#include <memory>

#include "../../core/player/Player.h"

class GameSession;

class GameRenderer {
public:
    GameRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(const GameSession& session);

private:
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int TILE_SIZE = 20;
    static constexpr int HUD_HEIGHT = 80;

    SDL_Renderer* renderer;
    AssetManager* assets;
    std::unique_ptr<PixelSpriteRenderer> spriteRenderer;

    std::string getPlayerSpriteName(const Player& player);
    bool isPlayerMoving(const Player& player);
    bool isPlayerFacingLeft(const Player& player);

    void renderBackground();
    void renderHUD(const GameSession& session);
    void renderGameArea(const GameSession& session);
    void renderPlayer(const GameSession& session);
    void renderBuildings(const GameSession& session);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color);
    void renderRect(int x, int y, int w, int h, SDL_Color color);
    void renderHUDPanel();
    void renderScore(int score);
    void renderCombo(int combo);
    void renderSpecialGauge(int gauge);
    void renderLives(int lives);
    void renderNumberImages(const std::string& numberStr, int startX, int y);
    void renderFallbackBackground();
    
    int gameToScreenX(int gameX) const;
    int gameToScreenY(float gameY) const;
};
