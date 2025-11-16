#pragma once

#include "../../interfaces/IRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

class GameSession;
struct GameOverDisplayData;

class SDLRenderer : public IRenderer {
public:
    SDLRenderer();
    ~SDLRenderer();

    bool initialize();
    void shutdown();

    void renderMenu(int highScore) override;
    void renderPlaying(const GameSession& session) override;
    void renderGameOver(const GameOverDisplayData& data) override;
    void clearScreen() override;
    void clearScreenFull() override;
    void flushOutput() override;

private:
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int TILE_SIZE = 20;
    static constexpr int HUD_HEIGHT = 80;

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    struct PixelSprite {
        std::vector<std::vector<int>> data;
        int width;
        int height;
        SDL_Color baseColor;
    };

    PixelSprite playerSprite;
    PixelSprite playerAttackSprite;
    PixelSprite playerDamagedSprite;
    PixelSprite buildingSprite;
    PixelSprite explosionSprite;

    bool initSDL();
    bool initTTF();
    void initializeSprites();

    void renderBackground();
    void renderPlayer(const GameSession& session);
    void renderBuildings(const GameSession& session);
    void renderHUD(const GameSession& session);
    void renderText(const std::string& text, int x, int y, SDL_Color color);
    void renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color);

    void renderPixelSprite(const PixelSprite& sprite, int x, int y, int scale);
    void renderRect(int x, int y, int w, int h, SDL_Color color);

    int gameToScreenX(int gameX) const;
    int gameToScreenY(float gameY) const;

    SDL_Color getPixelColor(const PixelSprite& sprite, int pixelType);
};
