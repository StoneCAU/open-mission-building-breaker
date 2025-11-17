#pragma once
#include <SDL2/SDL.h>
#include <memory>

class AssetManager;
class PlayerAnimationRenderer;
class HUDRenderer;
class BuildingRenderer;
class UltimateEffectRenderer;
class GameSession;
class Player;
class Building;
enum class PlayerActionType;

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
    static constexpr int HUD_HEIGHT = 120;
    static constexpr int TILE_SIZE = 20;
    static constexpr int MESSAGE_X = 250;
    static constexpr int MESSAGE_Y_OFFSET = 40;
    static constexpr int PLAYER_SPRITE_OFFSET_X = 8;
    static constexpr int PLAYER_SPRITE_OFFSET_Y = 16;
    static constexpr int GAME_AREA_OFFSET_X = 50;

    void renderBackground();
    void renderFallbackBackground();
    void renderGameArea(const GameSession& session);
    void renderPlayer(const GameSession& session);
    void renderBuildings(const GameSession& session);

    void handlePlayerSounds(const Player& player);
    void handleBuildingSounds(const GameSession& session);
    void processPlayerActionSounds(const Player& player);
    void processPlayerHitSound(const Player& player);
    void processBuildingDestroySounds(const GameSession& session);
    void processBuildingReboundSounds(const GameSession& session);

    struct PlayerRenderData {
        int screenX;
        int screenY;
    };

    PlayerRenderData calculatePlayerRenderData(const Player& player) const;
    void drawPlayer(const Player& player, const PlayerRenderData& data);
    void drawBuildings(const GameSession& session);

    int gameToScreenX(int gameX) const;
    int gameToScreenY(float gameY) const;

    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color);
    void renderRect(int x, int y, int w, int h, SDL_Color color);
};
