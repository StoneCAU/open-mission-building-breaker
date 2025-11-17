#pragma once
#include "../../interfaces/IRenderer.h"
#include <SDL2/SDL.h>
#include <memory>

class AssetManager;
class GameSession;
class PlayerAnimationRenderer;
class HUDRenderer;
class BuildingRenderer;
class UltimateEffectRenderer;

class GameRenderer {
public:
    GameRenderer(SDL_Renderer* renderer, AssetManager* assets);
    ~GameRenderer();

    void render(const GameSession& session);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;
    std::unique_ptr<PlayerAnimationRenderer> playerRenderer;
    std::unique_ptr<HUDRenderer> hudRenderer;
    std::unique_ptr<BuildingRenderer> buildingRenderer;
    std::unique_ptr<UltimateEffectRenderer> ultimateRenderer;

    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int HUD_HEIGHT = 100;
    static constexpr int TILE_SIZE = 12;

    void renderBackground();
    void renderGameArea(const GameSession& session);
    void renderPlayer(const GameSession& session);
    void renderBuildings(const GameSession& session);
    void renderFallbackBackground();
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color);
    void renderRect(int x, int y, int w, int h, SDL_Color color);

    int gameToScreenX(int gameX) const;
    int gameToScreenY(float gameY) const;
};
