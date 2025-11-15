#include "SDLRenderer.h"
#include "../../core/game/GameSession.h"
#include "../../core/game/GameOverDisplayData.h"
#include "../../core/game/GameConfig.h"
#include "../../core/player/Player.h"
#include "../../core/building/Building.h"
#include <iostream>

SDLRenderer::SDLRenderer()
    : window(nullptr), renderer(nullptr) {}

SDLRenderer::~SDLRenderer() {
    shutdown();
}

bool SDLRenderer::initialize() {
    return initSDL();
}

bool SDLRenderer::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL 초기화 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Building Breaker",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "윈도우 생성 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "렌더러 생성 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "SDL2 초기화 성공!" << std::endl;
    return true;
}

void SDLRenderer::shutdown() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_RenderClear(renderer);
}

void SDLRenderer::clearScreenFull() {
    clearScreen();
}

void SDLRenderer::flushOutput() {
    SDL_RenderPresent(renderer);
}

void SDLRenderer::renderBackground() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    int groundY = GameConfig::MAP_GROUND_Y * TILE_SIZE;
    SDL_RenderDrawLine(renderer, 0, groundY, WINDOW_WIDTH, groundY);
}

void SDLRenderer::renderPlayer(const GameSession& session) {
    const Player& player = session.getPlayer();
    
    int x = player.getX() * TILE_SIZE;
    int y = static_cast<int>(player.getY()) * TILE_SIZE;
    
    if (player.isDamaged()) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
    }
    
    SDL_Rect playerRect = {x, y - TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &playerRect);
}

void SDLRenderer::renderBuildings(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();
    
    for (const auto& building : buildings) {
        if (building.isDestroyed()) continue;
        
        int bx = building.getX() * TILE_SIZE;
        int by = static_cast<int>(building.getY()) * TILE_SIZE;
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        
        for (int i = 0; i < building.getHeight(); ++i) {
            SDL_Rect floorRect = {
                bx,
                by - (i * TILE_SIZE),
                Building::WIDTH * TILE_SIZE / 5,
                TILE_SIZE
            };
            SDL_RenderFillRect(renderer, &floorRect);
            
            SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
            SDL_RenderDrawRect(renderer, &floorRect);
            SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        }
    }
}

void SDLRenderer::renderHUD(const GameSession& session) {
    static int frameCount = 0;
    if (++frameCount % 60 == 0) {
        std::cout << "점수: " << session.getScore() 
                  << " | 콤보: " << session.getCombo()
                  << " | 게이지: " << session.getGauge() << "%"
                  << " | 목숨: " << session.getLife() << std::endl;
    }
}

void SDLRenderer::renderPlaying(const GameSession& session) {
    clearScreen();
    renderBackground();
    renderBuildings(session);
    renderPlayer(session);
    renderHUD(session);
    flushOutput();
}

void SDLRenderer::renderMenu(int highScore) {
    clearScreen();
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect titleRect = {WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 50, 200, 100};
    SDL_RenderFillRect(renderer, &titleRect);
    
    flushOutput();
    
    std::cout << "=== BUILDING BREAKER ===" << std::endl;
    std::cout << "최고 기록: " << highScore << std::endl;
    std::cout << "[ENTER] 게임 시작" << std::endl;
}

void SDLRenderer::renderGameOver(const GameOverDisplayData& data) {
    clearScreen();
    
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_Rect gameOverRect = {WINDOW_WIDTH/2 - 150, WINDOW_HEIGHT/2 - 75, 300, 150};
    SDL_RenderFillRect(renderer, &gameOverRect);
    
    flushOutput();
    
    std::cout << "=== GAME OVER ===" << std::endl;
    std::cout << "최종 점수: " << data.finalScore << std::endl;
}
