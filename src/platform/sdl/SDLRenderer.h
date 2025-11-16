#pragma once

#include "../../interfaces/IRenderer.h"
#include <SDL2/SDL.h>
#include <memory>

class AssetManager;
class MenuRenderer;
class GameRenderer;
class GameOverRenderer;

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

    SDL_Window* window;
    SDL_Renderer* renderer;

    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<MenuRenderer> menuRenderer;
    std::unique_ptr<GameRenderer> gameRenderer;
    std::unique_ptr<GameOverRenderer> gameOverRenderer;

    bool initSDL();
};
