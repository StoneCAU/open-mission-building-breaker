#pragma once

#include "../../interfaces/IRenderer.h"
#include <SDL2/SDL.h>

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

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    bool initSDL();
    void renderBackground();
    void renderPlayer(const GameSession& session);
    void renderBuildings(const GameSession& session);
    void renderHUD(const GameSession& session);
};
