#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <SDL_ttf.h>

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

    static constexpr SDL_Color FALLBACK_BG_COLOR = {25, 25, 40, 255};
    static constexpr SDL_Color GROUND_LINE_COLOR = {100, 100, 100, 255};
    static constexpr SDL_Color MESSAGE_COLOR = {255, 255, 100, 255};

    static constexpr int ATTACK_SOUND_COOLDOWN = 15;
    static constexpr int HIT_SOUND_COOLDOWN = 50;
    static constexpr int DEFEND_BLOCK_FRAMES = 35;
    static constexpr int DEFEND_SOUND_COOLDOWN = 38;

    void renderBackground();
    void renderFallbackBackground();
    void renderGameArea(const GameSession& session);
    void renderPlayer(const GameSession& session);
    void renderBuildings(const GameSession& session);
    void renderGameMessage(const GameSession& session);
    void renderUltimateEffect(const GameSession& session);

    void handlePlayerSounds(const Player& player);
    void handleBuildingSounds(const GameSession& session);
    void processPlayerActionSounds(const Player& player);
    void processPlayerHitSound(const Player& player);
    void processBuildingDestroySounds(const GameSession& session);
    void processBuildingReboundSounds(const GameSession& session);
    void processFloorDestroySounds(const GameSession& session);

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
    SDL_Surface* createTextSurface(const std::string& text, SDL_Color color, TTF_Font* font);
    SDL_Texture* createTextTexture(SDL_Surface* surface);
    void renderTextTexture(SDL_Texture* texture, int x, int y, int width, int height);
    void cleanupTextResources(SDL_Surface* surface, SDL_Texture* texture);
    void renderRect(int x, int y, int w, int h, SDL_Color color);
};
