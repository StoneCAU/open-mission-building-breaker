#pragma once
#include "../../interfaces/IRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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

    void handleMenuInput(InputKey key) override;
    void handleGameOverInput(InputKey key) override;

private:
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;

    static constexpr int AUDIO_FREQUENCY = 44100;
    static constexpr int AUDIO_CHANNELS = 2;
    static constexpr int AUDIO_CHUNK_SIZE = 2048;

    static constexpr SDL_Color CLEAR_COLOR = {25, 25, 40, 255};

    static constexpr const char* WINDOW_TITLE = "Building Breaker - Pixel Art Edition";

    SDL_Window* window;
    SDL_Renderer* renderer;

    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<MenuRenderer> menuRenderer;
    std::unique_ptr<GameRenderer> gameRenderer;
    std::unique_ptr<GameOverRenderer> gameOverRenderer;

    bool initSDL();
    bool initSDLSystems();
    bool initAudio();
    bool initWindow();
    bool initRenderer();
    bool initAssets();
    bool initRenderers();

    void cleanupRenderers();
    void cleanupSDLResources();
    void cleanupSDLSystems();
};
