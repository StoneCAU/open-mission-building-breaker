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
    SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    int groundY = gameToScreenY(GameConfig::MAP_GROUND_Y);
    SDL_RenderDrawLine(renderer, 0, groundY, WINDOW_WIDTH, groundY);
}

void GameRenderer::renderHUD(const GameSession& session) {
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
