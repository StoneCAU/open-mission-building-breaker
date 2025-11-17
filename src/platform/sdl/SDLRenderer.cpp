#include "SDLRenderer.h"
#include "AssetManager.h"
#include "MenuRenderer.h"
#include "GameRenderer.h"
#include "GameOverRenderer.h"
#include <iostream>

#include "SoundManager.h"

SDLRenderer::SDLRenderer()
    : window(nullptr), renderer(nullptr) {}

SDLRenderer::~SDLRenderer() {
    shutdown();
}

bool SDLRenderer::initialize() {
    if (!initSDL()) return false;

    assetManager = std::make_unique<AssetManager>(renderer);
    if (!assetManager->loadFonts() || !assetManager->loadTextures()) {
        return false;
    }

    // 오디오 로딩 확인
    if (!assetManager->loadAudio()) {
        std::cerr << "오디오 로딩 실패!" << std::endl;
        return false;
    }

    SoundManager::initialize(assetManager.get());
    menuRenderer = std::make_unique<MenuRenderer>(renderer, assetManager.get());
    gameRenderer = std::make_unique<GameRenderer>(renderer, assetManager.get());
    gameOverRenderer = std::make_unique<GameOverRenderer>(renderer, assetManager.get());

    std::cout << "SDL2 레트로 렌더러 초기화 완료!" << std::endl;
    return true;
}

bool SDLRenderer::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL 초기화 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "TTF 초기화 실패: " << TTF_GetError() << std::endl;
        return false;
    }

    // SDL_mixer 초기화 (여기가 핵심!)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer 초기화 실패: " << Mix_GetError() << std::endl;
        return false;
    }
    std::cout << "SDL_mixer 초기화 성공!" << std::endl;

    window = SDL_CreateWindow("Building Breaker - Pixel Art Edition",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "윈도우 생성 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        std::cerr << "렌더러 생성 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
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
    SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
    SDL_RenderClear(renderer);
}

void SDLRenderer::clearScreenFull() {
    clearScreen();
}

void SDLRenderer::flushOutput() {
    SDL_RenderPresent(renderer);
}

void SDLRenderer::shutdown() {
    gameOverRenderer.reset();
    gameRenderer.reset();
    menuRenderer.reset();
    assetManager.reset();

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
