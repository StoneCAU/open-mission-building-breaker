#include "GameRenderer.h"
#include "../../core/game/GameSession.h"
#include "../../core/game/GameConfig.h"
#include "../../core/player/Player.h"
#include "../../core/building/Building.h"

GameRenderer::GameRenderer(SDL_Renderer* r, AssetManager* a) 
    : renderer(r), assets(a) {
    spriteRenderer = std::make_unique<PixelSpriteRenderer>(renderer);
    spriteRenderer->initializeSprites();
}

void GameRenderer::render(const GameSession& session) {
    renderBackground();
    renderGameArea(session);
    renderHUD(session);
    
    if (session.messageQueue.hasMessage()) {
        std::string message = session.messageQueue.getMessage();
        renderTextCentered(message, WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100, {255, 255, 100, 255});
    }
    
    SDL_RenderPresent(renderer);
}

void GameRenderer::renderBackground() {
    SDL_Texture* bgTexture = assets->getTexture("game_bg");
    if (bgTexture) {
        SDL_Rect fullScreen = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &fullScreen);
    } else {
        renderFallbackBackground();
    }
}

void GameRenderer::renderFallbackBackground() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    int groundY = gameToScreenY(GameConfig::MAP_GROUND_Y);
    SDL_RenderDrawLine(renderer, 0, groundY, WINDOW_WIDTH, groundY);
}

void GameRenderer::renderHUD(const GameSession& session) {
    //renderHUDPanel();
    renderScore(session.getScore());
    renderCombo(session.getCombo());
    renderSpecialGauge(session.getGauge());
    renderLives(session.getLife());
}

void GameRenderer::renderHUDPanel() {
    SDL_Texture* uiPanel = assets->getTexture("ui_panel");
    if (uiPanel) {
        SDL_Rect panelRect = {0, 0, WINDOW_WIDTH, 100};
        SDL_RenderCopy(renderer, uiPanel, nullptr, &panelRect);
    } else {
        renderRect(0, 0, WINDOW_WIDTH, HUD_HEIGHT, {0, 0, 0, 180});
    }
}

void GameRenderer::renderScore(int score) {
    SDL_Texture* scoreFrame = assets->getTexture("score_frame");
    if (scoreFrame) {
        SDL_Rect frameRect = {20, 15, 160, 60};
        SDL_RenderCopy(renderer, scoreFrame, nullptr, &frameRect);

        // 점수를 프레임 우측에 정렬
        std::string scoreStr = std::to_string(score);
        int scoreWidth = scoreStr.length() * 12;
        int rightMargin = 40; // 15→25로 늘림
        int scoreX = (20 + 160) - scoreWidth - rightMargin;

        renderNumberImages(scoreStr, scoreX, 35);
    }
}

void GameRenderer::renderCombo(int combo) {
    SDL_Texture* comboMedal = assets->getTexture("combo_medal");
    if (comboMedal) {
        // 콤보 메달 크기 조정
        SDL_Rect medalRect = {200, 15, 60, 60}; // 220→200 (왼쪽으로), 70→60 (크기 축소)
        SDL_RenderCopy(renderer, comboMedal, nullptr, &medalRect);

        // 콤보 숫자를 메달 중앙에
        std::string comboStr = std::to_string(combo);
        int comboWidth = comboStr.length() * 14;
        int medalCenterX = 200 + (60 - comboWidth) / 2;
        renderNumberImages(comboStr, medalCenterX, 35);
    }
}

void GameRenderer::renderSpecialGauge(int gauge) {
    SDL_Texture* emptyGauge = assets->getTexture("gauge_empty");
    SDL_Texture* fillGauge = assets->getTexture("gauge_fill");

    int gaugeX = 50;
    int gaugeY = WINDOW_HEIGHT - 40;
    int gaugeWidth = 160;  // 200→160으로 줄임 (비율 맞춤)
    int gaugeHeight = 20;

    if (emptyGauge) {
        SDL_Rect emptyRect = {gaugeX, gaugeY, gaugeWidth, gaugeHeight};
        SDL_RenderCopy(renderer, emptyGauge, nullptr, &emptyRect);
    }

    if (fillGauge && gauge > 0) {
        int fillWidth = (gauge * gaugeWidth) / 100;
        SDL_Rect srcRect = {0, 0, fillWidth, gaugeHeight};
        SDL_Rect dstRect = {gaugeX, gaugeY, fillWidth, gaugeHeight};
        SDL_RenderCopy(renderer, fillGauge, &srcRect, &dstRect);
    }
}

void GameRenderer::renderLives(int lives) {
    SDL_Texture* activeLife = assets->getTexture("life_active");
    SDL_Texture* inactiveLife = assets->getTexture("life_inactive");

    int startX = 70;          // 게이지 위쪽 중앙 정렬
    int y = WINDOW_HEIGHT - 70;  // 게이지 위에 배치 (30px 위)
    int maxLives = 3;

    if (activeLife && inactiveLife) {
        for (int i = 0; i < maxLives; ++i) {
            SDL_Texture* lifeTexture = (i < lives) ? activeLife : inactiveLife;
            SDL_Rect lifeRect = {startX + i * 28, y, 24, 24}; // 크기도 조정
            SDL_RenderCopy(renderer, lifeTexture, nullptr, &lifeRect);
        }
    }
}

// 숫자 크기도 조정
void GameRenderer::renderNumberImages(const std::string& numberStr, int startX, int y) {
    for (int i = 0; i < numberStr.length(); ++i) {
        if (numberStr[i] >= '0' && numberStr[i] <= '9') {
            int digit = numberStr[i] - '0';
            SDL_Texture* numberTexture = assets->getTexture("number_" + std::to_string(digit));
            if (numberTexture) {
                // 간격을 줄임: 18→12
                SDL_Rect numberRect = {startX + i * 12, y, 16, 20};
                SDL_RenderCopy(renderer, numberTexture, nullptr, &numberRect);
            }
        }
    }
}

void GameRenderer::renderGameArea(const GameSession& session) {
    renderBuildings(session);
    renderPlayer(session);
}

void GameRenderer::renderPlayer(const GameSession& session) {
    const Player& player = session.getPlayer();

    int screenX = gameToScreenX(player.getX()) - 8;
    int screenY = gameToScreenY(player.getY() + 1) - 16;

    if (player.isDamaged()) {
        spriteRenderer->renderSprite(spriteRenderer->getDamagedSprite(), screenX, screenY, 2);
        return;
    }

    if (player.getAction() == PlayerActionType::ATTACK && player.isAttackActiveFrame()) {
        spriteRenderer->renderSprite(spriteRenderer->getAttackSprite(), screenX, screenY, 2);
        return;
    }

    spriteRenderer->renderSprite(spriteRenderer->getPlayerSprite(), screenX, screenY, 2);

    if (player.getAction() == PlayerActionType::DEFEND) {
        renderRect(screenX - 6, screenY + 8, 4, 12, {139, 69, 19, 255});
    }
}

void GameRenderer::renderBuildings(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        if (building.isDestroyed()) continue;

        int baseX = gameToScreenX(building.getX());

        for (int floor = 0; floor < building.getHeight(); ++floor) {
            float buildingFloorY = building.getY() - floor;
            int screenY = gameToScreenY(buildingFloorY) - 12;

            for (int blockX = 0; blockX < Building::WIDTH; blockX += 2) {
                spriteRenderer->renderSprite(spriteRenderer->getBuildingSprite(),
                                           baseX + blockX * 6, screenY, 2);
            }
        }
    }
}

int GameRenderer::gameToScreenX(int gameX) const {
    return gameX * TILE_SIZE + 50;
}

int GameRenderer::gameToScreenY(float gameY) const {
    return WINDOW_HEIGHT - HUD_HEIGHT - static_cast<int>((GameConfig::MAP_GROUND_Y - gameY) * TILE_SIZE);
}

void GameRenderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont("game");
    if (!font) return;

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_Rect rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}

void GameRenderer::renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont("game");
    if (!font) return;

    int textWidth = 0;
    int textHeight = 0;
    TTF_SizeUTF8(font, text.c_str(), &textWidth, &textHeight);

    renderText(text, centerX - textWidth / 2, y, color);
}

void GameRenderer::renderRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}
