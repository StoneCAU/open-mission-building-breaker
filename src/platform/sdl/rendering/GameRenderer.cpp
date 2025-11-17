#include "GameRenderer.h"
#include "PlayerAnimationRenderer.h"
#include "HUDRenderer.h"
#include "BuildingRenderer.h"
#include "UltimateEffectRenderer.h"
#include "../../../core/game/GameSession.h"
#include "../../../core/game/GameConfig.h"
#include "../../../core/player/Player.h"
#include "../../../core/building/Building.h"
#include "../assets/AssetConfig.h"
#include "../assets/AssetManager.h"
#include "../assets/SoundManager.h"

GameRenderer::GameRenderer(SDL_Renderer* r, AssetManager* a)
    : renderer(r), assets(a) {
    playerRenderer = std::make_unique<PlayerAnimationRenderer>(assets);
    hudRenderer = std::make_unique<HUDRenderer>(renderer, assets);
    buildingRenderer = std::make_unique<BuildingRenderer>(renderer, assets);
    ultimateRenderer = std::make_unique<UltimateEffectRenderer>(assets);
}

GameRenderer::~GameRenderer() = default;

void GameRenderer::render(const GameSession& session) {
    SoundManager::playBGM(AssetConfig::MUSIC_GAME);

    ultimateRenderer->update(session);
    const_cast<GameSession&>(session).clearUltimateFlag();

    renderBackground();
    renderGameArea(session);
    renderUltimateEffect(session);

    hudRenderer->render(session);
    renderGameMessage(session);

    SoundManager::nextFrame();
    SDL_RenderPresent(renderer);
}

void GameRenderer::renderBackground() {
    SDL_Texture* bgTexture = assets->getTexture(AssetConfig::TEXTURE_GAME_BG);

    const auto renderGameBackground = [&]() {
        SDL_Rect fullScreen{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &fullScreen);
    };

    bgTexture && (renderGameBackground(), true) || (renderFallbackBackground(), true);
}

void GameRenderer::renderFallbackBackground() {
    SDL_SetRenderDrawColor(renderer, FALLBACK_BG_COLOR.r, FALLBACK_BG_COLOR.g,
                           FALLBACK_BG_COLOR.b, FALLBACK_BG_COLOR.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, GROUND_LINE_COLOR.r, GROUND_LINE_COLOR.g,
                           GROUND_LINE_COLOR.b, GROUND_LINE_COLOR.a);
    int groundY = gameToScreenY(GameConfig::MAP_GROUND_Y);
    SDL_RenderDrawLine(renderer, 0, groundY, WINDOW_WIDTH, groundY);
}

void GameRenderer::renderGameArea(const GameSession& session) {
    renderPlayer(session);
    renderBuildings(session);
}

void GameRenderer::renderUltimateEffect(const GameSession& session) {
    const Player& player = session.getPlayer();
    int centerX = gameToScreenX(player.getX());
    int centerY = gameToScreenY(player.getY());
    ultimateRenderer->render(renderer, centerX, centerY);
}

void GameRenderer::renderGameMessage(const GameSession& session) {
    const auto renderMessage = [&]() {
        std::string message = session.messageQueue.getMessage();
        int messageY = WINDOW_HEIGHT - MESSAGE_Y_OFFSET;
        renderText(message, MESSAGE_X, messageY, MESSAGE_COLOR);
    };

    session.messageQueue.hasMessage() && (renderMessage(), true);
}

void GameRenderer::renderPlayer(const GameSession& session) {
    const Player& player = session.getPlayer();

    handlePlayerSounds(player);

    PlayerRenderData renderData = calculatePlayerRenderData(player);
    drawPlayer(player, renderData);
}

void GameRenderer::renderBuildings(const GameSession& session) {
    handleBuildingSounds(session);
    drawBuildings(session);
}

void GameRenderer::handlePlayerSounds(const Player& player) {
    processPlayerActionSounds(player);
    processPlayerHitSound(player);
}

void GameRenderer::handleBuildingSounds(const GameSession& session) {
    processBuildingDestroySounds(session);
    processBuildingReboundSounds(session);
    processFloorDestroySounds(session);
}

void GameRenderer::processPlayerActionSounds(const Player& player) {
    static PlayerActionType lastAction = PlayerActionType::IDLE;
    PlayerActionType currentAction = player.getAction();

    const auto playAttackSound = [&]() {
        (currentAction == PlayerActionType::ATTACK) &&
            (SoundManager::playWithCooldown(AssetConfig::SOUND_ATTACK, ATTACK_SOUND_COOLDOWN), true);
    };

    (currentAction != lastAction) && (playAttackSound(), lastAction = currentAction, true);
}

void GameRenderer::processPlayerHitSound(const Player& player) {
    player.isDamaged() && (SoundManager::blockSound(AssetConfig::SOUND_DEFEND, DEFEND_BLOCK_FRAMES),
                           SoundManager::playWithCooldown(AssetConfig::SOUND_HIT, HIT_SOUND_COOLDOWN), true);
}

void GameRenderer::processBuildingDestroySounds(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        building.isDestroyed() &&
            (SoundManager::playOnce(AssetConfig::SOUND_BUILDING_COLLAPSE), true);
    }
}

void GameRenderer::processBuildingReboundSounds(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        building.isRebounded() &&
            (SoundManager::playWithCooldown(AssetConfig::SOUND_DEFEND, DEFEND_SOUND_COOLDOWN), true);
    }
}

void GameRenderer::processFloorDestroySounds(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        for (const auto& floor : building.getFloors()) {
            floor.isDestroyed() &&
                (SoundManager::playOnce(AssetConfig::SOUND_FLOOR_BREAK), true);
        }
    }
}

GameRenderer::PlayerRenderData GameRenderer::calculatePlayerRenderData(const Player& player) const {
    int screenX = gameToScreenX(player.getX()) - PLAYER_SPRITE_OFFSET_X;
    int screenY = gameToScreenY(player.getY() + 1) - PLAYER_SPRITE_OFFSET_Y;
    return {screenX, screenY};
}

void GameRenderer::drawPlayer(const Player& player, const PlayerRenderData& data) {
    playerRenderer->render(renderer, player, data.screenX, data.screenY);
}

void GameRenderer::drawBuildings(const GameSession& session) {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        const auto renderSingleBuilding = [&]() {
            int baseX = gameToScreenX(building.getX());
            int baseY = gameToScreenY(building.getY());
            buildingRenderer->renderBuilding(building, baseX, baseY);
        };

        building.isDestroyed() || (renderSingleBuilding(), true);
    }
}

int GameRenderer::gameToScreenX(int gameX) const {
    return gameX * TILE_SIZE + GAME_AREA_OFFSET_X;
}

int GameRenderer::gameToScreenY(float gameY) const {
    return WINDOW_HEIGHT - HUD_HEIGHT - static_cast<int>((GameConfig::MAP_GROUND_Y - gameY) * TILE_SIZE);
}

void GameRenderer::renderText(const std::string& text, int x, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont(AssetConfig::FONT_GAME);

    const auto createAndRenderText = [&]() {
        SDL_Surface* surface = createTextSurface(text, color, font);

        const auto processTextSurface = [&]() {
            SDL_Texture* texture = createTextTexture(surface);

            const auto renderAndCleanup = [&]() {
                renderTextTexture(texture, x, y, surface->w, surface->h);
                cleanupTextResources(surface, texture);
            };

            texture && (renderAndCleanup(), true);
        };

        surface && (processTextSurface(), true);
    };

    font && (createAndRenderText(), true);
}

SDL_Surface* GameRenderer::createTextSurface(const std::string& text, SDL_Color color, TTF_Font* font) {
    return TTF_RenderUTF8_Solid(font, text.c_str(), color);
}

SDL_Texture* GameRenderer::createTextTexture(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
}

void GameRenderer::renderTextTexture(SDL_Texture* texture, int x, int y, int width, int height) {
    SDL_Rect rect{x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void GameRenderer::cleanupTextResources(SDL_Surface* surface, SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void GameRenderer::renderTextCentered(const std::string& text, int centerX, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont(AssetConfig::FONT_GAME);

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
