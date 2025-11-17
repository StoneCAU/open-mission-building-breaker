#include "GameRenderer.h"
#include "PlayerAnimationRenderer.h"
#include "HUDRenderer.h"
#include "BuildingRenderer.h"
#include "UltimateEffectRenderer.h"
#include "AssetManager.h"
#include "SoundManager.h"
#include "../../core/game/GameSession.h"
#include "../../core/game/GameConfig.h"
#include "../../core/player/Player.h"
#include "../../core/building/Building.h"

GameRenderer::GameRenderer(SDL_Renderer* r, AssetManager* a)
    : renderer(r), assets(a) {
    playerRenderer = std::make_unique<PlayerAnimationRenderer>(assets);
    hudRenderer = std::make_unique<HUDRenderer>(renderer, assets);
    buildingRenderer = std::make_unique<BuildingRenderer>(renderer, assets);
    ultimateRenderer = std::make_unique<UltimateEffectRenderer>(assets);
}

GameRenderer::~GameRenderer() = default;

void GameRenderer::render(const GameSession& session) {
    SoundManager::playBGM("game");

    ultimateRenderer->update(session);
    const_cast<GameSession&>(session).clearUltimateFlag();

    renderBackground();
    renderGameArea(session);

    const Player& player = session.getPlayer();
    int centerX = gameToScreenX(player.getX());
    int centerY = gameToScreenY(player.getY());
    ultimateRenderer->render(renderer, centerX, centerY);

    hudRenderer->render(session);

    const auto renderMessage = [&]() {
        std::string message = session.messageQueue.getMessage();
        int messageX = 250;
        int messageY = WINDOW_HEIGHT - 40;
        renderText(message, messageX, messageY, {255, 255, 100, 255});
    };

    session.messageQueue.hasMessage() && (renderMessage(), true);

    SoundManager::nextFrame();
    SDL_RenderPresent(renderer);
}

void GameRenderer::renderBackground() {
    SDL_Texture* bgTexture = assets->getTexture("game_bg");

    const auto renderGameBackground = [&]() {
        SDL_Rect fullScreen{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &fullScreen);
    };

    bgTexture && (renderGameBackground(), true) || (renderFallbackBackground(), true);
}

void GameRenderer::renderFallbackBackground() {
    SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    int groundY = gameToScreenY(GameConfig::MAP_GROUND_Y);
    SDL_RenderDrawLine(renderer, 0, groundY, WINDOW_WIDTH, groundY);
}

void GameRenderer::renderGameArea(const GameSession& session) {
    renderPlayer(session);
    renderBuildings(session);
}

void GameRenderer::renderPlayer(const GameSession& session) {
    const Player& player = session.getPlayer();
    int screenX = gameToScreenX(player.getX()) - 8;
    int screenY = gameToScreenY(player.getY() + 1) - 16;

    static PlayerActionType lastAction = PlayerActionType::IDLE;
    PlayerActionType currentAction = player.getAction();

    const auto playAttackSound = [&]() {
        (currentAction == PlayerActionType::ATTACK) &&
            (SoundManager::playWithCooldown("attack", 15), true);
    };

    (currentAction != lastAction) && (playAttackSound(), lastAction = currentAction, true);

    player.isDamaged() && (SoundManager::blockSound("defend", 35),
                           SoundManager::playWithCooldown("hit", 50), true);

    playerRenderer->render(renderer, player, screenX, screenY);
}

void GameRenderer::renderBuildings(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        building.isDestroyed() &&
            (SoundManager::playOnce("building_collapse"), true);
    }

    for (const auto& building : buildings) {
        const auto renderSingleBuilding = [&]() {
            int baseX = gameToScreenX(building.getX());
            int baseY = gameToScreenY(building.getY());
            buildingRenderer->renderBuilding(building, baseX, baseY);
        };

        building.isRebounded() &&
            (SoundManager::playWithCooldown("defend", 38), true);

        building.isDestroyed() || (renderSingleBuilding(), true);
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

    const auto createAndRenderTexture = [&]() {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);

        const auto renderSurface = [&]() {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            const auto copyTexture = [&]() {
                SDL_Rect rect{x, y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
                SDL_DestroyTexture(texture);
            };

            texture && (copyTexture(), true);
            SDL_FreeSurface(surface);
        };

        surface && (renderSurface(), true);
    };

    font && (createAndRenderTexture(), true);
}

void GameRenderer::renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont("game");

    const auto calculateAndRender = [&]() {
        int textWidth = 0;
        int textHeight = 0;
        TTF_SizeUTF8(font, text.c_str(), &textWidth, &textHeight);
        renderText(text, centerX - textWidth / 2, y, color);
    };

    font && (calculateAndRender(), true);
}

void GameRenderer::renderRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect{x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}
