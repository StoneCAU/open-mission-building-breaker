#include "SDLRenderer.h"
#include "../../core/game/GameSession.h"
#include "../../core/game/GameOverDisplayData.h"
#include "../../core/game/GameConfig.h"
#include "../../core/player/Player.h"
#include "../../core/building/Building.h"
#include <iostream>

SDLRenderer::SDLRenderer()
    : window(nullptr), renderer(nullptr), font(nullptr) {}

SDLRenderer::~SDLRenderer() {
    shutdown();
}

bool SDLRenderer::initialize() {
    if (!initSDL()) {
        return false;
    }

    if (!initTTF()) {
        return false;
    }

    initializeSprites();
    std::cout << "SDL2 + 픽셀아트 초기화 성공!" << std::endl;
    return true;
}

bool SDLRenderer::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL 초기화 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Building Breaker - Pixel Art Edition",
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

    return true;
}

bool SDLRenderer::initTTF() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF 초기화 실패: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("C:/Windows/Fonts/malgun.ttf", 18);
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/gulim.ttf", 18);
        if (!font) {
            font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 18);
        }
    }

    if (!font) {
        std::cerr << "폰트 로드 실패: " << TTF_GetError() << std::endl;
        return false;
    }

    return true;
}

void SDLRenderer::initializeSprites() {
    playerSprite = {
        {
            {0, 0, 1, 1, 1, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 0},
            {1, 1, 2, 1, 1, 2, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 3, 3, 1, 1, 1},
            {0, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 1, 0, 0, 1, 0, 0},
            {0, 1, 1, 0, 0, 1, 1, 0}
        },
        8, 8, {80, 120, 200, 255}
    };

    playerAttackSprite = {
        {
            {0, 0, 1, 1, 1, 1, 0, 0},
            {0, 1, 1, 1, 1, 1, 1, 0},
            {1, 1, 2, 1, 1, 2, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 4, 4, 1, 1, 1},
            {0, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 1, 0, 0, 1, 0, 0},
            {0, 1, 1, 0, 0, 1, 1, 0}
        },
        8, 8, {80, 120, 200, 255}
    };

    playerDamagedSprite = {
        playerSprite.data,
        8, 8, {220, 80, 80, 255}
    };

    buildingSprite = {
        {
            {1, 1, 1, 1, 1, 1},
            {1, 2, 2, 2, 2, 1},
            {1, 2, 0, 0, 2, 1},
            {1, 2, 0, 0, 2, 1},
            {1, 2, 2, 2, 2, 1},
            {1, 1, 1, 1, 1, 1}
        },
        6, 6, {120, 120, 120, 255}
    };

    explosionSprite = {
        {
            {0, 0, 1, 2, 2, 1, 0, 0},
            {0, 1, 2, 3, 3, 2, 1, 0},
            {1, 2, 3, 4, 4, 3, 2, 1},
            {2, 3, 4, 4, 4, 4, 3, 2},
            {2, 3, 4, 4, 4, 4, 3, 2},
            {1, 2, 3, 4, 4, 3, 2, 1},
            {0, 1, 2, 3, 3, 2, 1, 0},
            {0, 0, 1, 2, 2, 1, 0, 0}
        },
        8, 8, {255, 150, 0, 255}
    };
}

SDL_Color SDLRenderer::getPixelColor(const PixelSprite& sprite, int pixelType) {
    if (pixelType == 1) {
        return sprite.baseColor;
    }

    if (pixelType == 2) {
        return {220, 220, 220, 255};
    }

    if (pixelType == 3) {
        return {100, 100, 100, 255};
    }

    if (pixelType == 4) {
        return {255, 255, 100, 255};
    }

    return sprite.baseColor;
}

void SDLRenderer::renderPixelSprite(const PixelSprite& sprite, int x, int y, int scale) {
    for (int row = 0; row < sprite.height; ++row) {
        for (int col = 0; col < sprite.width; ++col) {
            int pixelType = sprite.data[row][col];
            if (pixelType == 0) {
                continue;
            }

            SDL_Color color = getPixelColor(sprite, pixelType);
            renderRect(x + col * scale, y + row * scale, scale, scale, color);
        }
    }
}

void SDLRenderer::renderRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void SDLRenderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    if (!font) {
        return;
    }

    SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (!textSurface) {
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void SDLRenderer::renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color) {
    if (!font) {
        return;
    }

    int textWidth = 0;
    int textHeight = 0;
    TTF_SizeUTF8(font, text.c_str(), &textWidth, &textHeight);

    renderText(text, centerX - textWidth / 2, y, color);
}

int SDLRenderer::gameToScreenX(int gameX) const {
    return gameX * TILE_SIZE + 50;
}

int SDLRenderer::gameToScreenY(float gameY) const {
    return WINDOW_HEIGHT - HUD_HEIGHT - static_cast<int>((GameConfig::MAP_GROUND_Y - gameY) * TILE_SIZE);
}

void SDLRenderer::shutdown() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
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

void SDLRenderer::renderBackground() {
    clearScreen();

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    int groundY = gameToScreenY(GameConfig::MAP_GROUND_Y);
    SDL_RenderDrawLine(renderer, 0, groundY, WINDOW_WIDTH, groundY);
}

void SDLRenderer::renderPlayer(const GameSession& session) {
    const Player& player = session.getPlayer();

    int screenX = gameToScreenX(player.getX()) - 8;
    int screenY = gameToScreenY(player.getY() + 1) - 16;

    if (player.isDamaged()) {
        renderPixelSprite(playerDamagedSprite, screenX, screenY, 2);
        return;
    }

    if (player.getAction() == PlayerActionType::ATTACK && player.isAttackActiveFrame()) {
        renderPixelSprite(playerAttackSprite, screenX, screenY, 2);
        return;
    }

    renderPixelSprite(playerSprite, screenX, screenY, 2);

    if (player.getAction() == PlayerActionType::DEFEND) {
        renderRect(screenX - 6, screenY + 8, 4, 12, {139, 69, 19, 255});
    }
}

void SDLRenderer::renderBuildings(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        if (building.isDestroyed()) {
            continue;
        }

        int baseX = gameToScreenX(building.getX());

        for (int floor = 0; floor < building.getHeight(); ++floor) {
            float buildingFloorY = building.getY() - floor;
            int screenY = gameToScreenY(buildingFloorY) - 12;

            for (int blockX = 0; blockX < Building::WIDTH; blockX += 2) {
                renderPixelSprite(buildingSprite, baseX + blockX * 6, screenY, 2);
            }
        }
    }
}

void SDLRenderer::renderHUD(const GameSession& session) {
    renderRect(0, 0, WINDOW_WIDTH, HUD_HEIGHT, {0, 0, 0, 180});

    std::string scoreText = "점수: " + std::to_string(session.getScore());
    renderText(scoreText, 20, 20, {255, 255, 255, 255});

    std::string comboText = "콤보: x" + std::to_string(session.getCombo());
    renderText(comboText, 200, 20, {255, 255, 100, 255});

    renderText("게이지:", 380, 20, {255, 255, 255, 255});
    renderRect(450, 25, 100, 15, {60, 60, 60, 255});

    int gaugeWidth = session.getGauge();
    renderRect(450, 25, gaugeWidth, 15, {100, 255, 100, 255});

    renderText("생명:", 580, 20, {255, 255, 255, 255});
    for (int i = 0; i < session.getLife(); ++i) {
        renderRect(630 + i * 25, 25, 20, 15, {255, 100, 100, 255});
    }
}

void SDLRenderer::renderPlaying(const GameSession& session) {
    renderBackground();
    renderBuildings(session);
    renderPlayer(session);
    renderHUD(session);

    if (session.messageQueue.hasMessage()) {
        std::string message = session.messageQueue.getMessage();
        renderTextCentered(message, WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100, {255, 255, 100, 255});
    }

    flushOutput();
}

void SDLRenderer::renderMenu(int highScore) {
    clearScreen();

    renderTextCentered("BUILDING BREAKER", WINDOW_WIDTH / 2, 200, {255, 255, 255, 255});
    renderTextCentered("Pixel Art Edition", WINDOW_WIDTH / 2, 230, {150, 150, 255, 255});

    std::string scoreText = "최고 기록: " + std::to_string(highScore) + "점";
    renderTextCentered(scoreText, WINDOW_WIDTH / 2, 300, {255, 255, 100, 255});

    renderTextCentered("[ENTER] 게임 시작", WINDOW_WIDTH / 2, 400, {100, 255, 100, 255});
    renderTextCentered("[Q] 종료", WINDOW_WIDTH / 2, 430, {255, 100, 100, 255});

    flushOutput();
}

void SDLRenderer::renderGameOver(const GameOverDisplayData& data) {
    clearScreen();

    renderTextCentered("게임 오버", WINDOW_WIDTH / 2, 180, {255, 100, 100, 255});

    std::string finalScore = "최종 점수: " + std::to_string(data.finalScore) + "점";
    renderTextCentered(finalScore, WINDOW_WIDTH / 2, 280, {255, 255, 255, 255});

    std::string maxCombo = "최고 콤보: x" + std::to_string(data.maxCombo);
    renderTextCentered(maxCombo, WINDOW_WIDTH / 2, 310, {255, 255, 100, 255});

    if (data.isNewRecord) {
        renderTextCentered("새로운 최고 기록!", WINDOW_WIDTH / 2, 350, {255, 255, 100, 255});
    }

    renderTextCentered("[R] 재시작", WINDOW_WIDTH / 2, 420, {100, 255, 100, 255});
    renderTextCentered("[Q] 종료", WINDOW_WIDTH / 2, 450, {255, 100, 100, 255});

    flushOutput();
}
