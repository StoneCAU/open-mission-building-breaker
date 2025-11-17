#include "SDLRenderer.h"
#include "assets/AssetManager.h"
#include "assets/SoundManager.h"
#include "rendering/GameOverRenderer.h"
#include "rendering/GameRenderer.h"
#include "rendering/MenuRenderer.h"

SDLRenderer::SDLRenderer()
    : window(nullptr), renderer(nullptr) {}

SDLRenderer::~SDLRenderer() {
    shutdown();
}

bool SDLRenderer::initialize() {
    bool success = initSDL();
    success && (success = initAssets(), true);
    success && (success = initRenderers(), true);

    return success;
}

bool SDLRenderer::initSDL() {
    bool success = initSDLSystems();
    success && (success = initAudio(), true);
    success && (success = initWindow(), true);
    success && (success = initRenderer(), true);

    return success;
}

bool SDLRenderer::initSDLSystems() {
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) >= 0 &&
           TTF_Init() >= 0;
}

bool SDLRenderer::initAudio() {
    return Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNK_SIZE) >= 0;
}

bool SDLRenderer::initWindow() {
    window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    return window != nullptr;
}

bool SDLRenderer::initRenderer() {
    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    return renderer != nullptr;
}

bool SDLRenderer::initAssets() {
    assetManager = std::make_unique<AssetManager>(renderer);

    bool success = assetManager->loadFonts();
    success && (success = assetManager->loadTextures(), true);
    success && (success = assetManager->loadAudio(), true);

    success && (SoundManager::initialize(assetManager.get()), true);

    return success;
}

bool SDLRenderer::initRenderers() {
    menuRenderer = std::make_unique<MenuRenderer>(renderer, assetManager.get());
    gameRenderer = std::make_unique<GameRenderer>(renderer, assetManager.get());
    gameOverRenderer = std::make_unique<GameOverRenderer>(renderer, assetManager.get());

    return menuRenderer && gameRenderer && gameOverRenderer;
}

void SDLRenderer::renderMenu(int highScore) {
    menuRenderer->render(highScore);
}

void SDLRenderer::renderPlaying(const GameSession& session) {
    gameRenderer->render(session);
}

void SDLRenderer::renderGameOver(const GameOverDisplayData& data) {
    gameOverRenderer->render(data);
}

void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, CLEAR_COLOR.r, CLEAR_COLOR.g,
                           CLEAR_COLOR.b, CLEAR_COLOR.a);
    SDL_RenderClear(renderer);
}

void SDLRenderer::clearScreenFull() {
    clearScreen();
}

void SDLRenderer::flushOutput() {
    SDL_RenderPresent(renderer);
}

void SDLRenderer::shutdown() {
    cleanupRenderers();
    cleanupSDLResources();
    cleanupSDLSystems();
}

void SDLRenderer::cleanupRenderers() {
    gameOverRenderer.reset();
    gameRenderer.reset();
    menuRenderer.reset();
    assetManager.reset();
}

void SDLRenderer::cleanupSDLResources() {
    renderer && (SDL_DestroyRenderer(renderer), renderer = nullptr, true);
    window && (SDL_DestroyWindow(window), window = nullptr, true);
}

void SDLRenderer::cleanupSDLSystems() {
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

void SDLRenderer::handleMenuInput(InputKey key) {
    menuRenderer->handleInput(key);
}

void SDLRenderer::handleGameOverInput(InputKey key) {
    gameOverRenderer->handleInput(key);
}
