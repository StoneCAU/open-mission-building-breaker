#include "GameOverRenderer.h"
#include "../../../core/game/GameOverDisplayData.h"
#include "../../../interfaces/InputKey.h"
#include "../assets/AssetConfig.h"
#include "../assets/AssetManager.h"
#include "../assets/SoundManager.h"

GameOverRenderer::GameOverRenderer(SDL_Renderer* r, AssetManager* a)
    : renderer(r), assets(a) {}

void GameOverRenderer::render(const GameOverDisplayData& data) {
    SoundManager::playBGM(AssetConfig::MUSIC_GAMEOVER);

    renderBackground();
    renderGameOverContent(data);

    SoundManager::nextFrame();
    SDL_RenderPresent(renderer);
}

void GameOverRenderer::renderGameOverContent(const GameOverDisplayData& data) {
    renderTextCentered(TEXT_GAME_OVER, CENTER_X, TITLE_Y, TITLE_COLOR);

    renderHighScore(data.highScore);

    renderScoreSection(data);

    data.isNewRecord && (renderTextCentered(TEXT_NEW_RECORD, CENTER_X, NEW_RECORD_Y, RECORD_COLOR), true);

    renderControlButtons();
}

void GameOverRenderer::renderHighScore(int highScore) {
    std::string text = std::string(TEXT_HIGH_SCORE) + std::to_string(highScore) + TEXT_SCORE_UNIT;
    renderTextCentered(text, CENTER_X, HIGH_SCORE_Y, HIGH_SCORE_COLOR);
}

void GameOverRenderer::renderScoreSection(const GameOverDisplayData& data) {
    std::string finalScore = formatScoreText(data.finalScore);
    renderTextCentered(finalScore, CENTER_X, FINAL_SCORE_Y, SCORE_COLOR);

    std::string maxCombo = formatComboText(data.maxCombo);
    renderTextCentered(maxCombo, CENTER_X, MAX_COMBO_Y, COMBO_COLOR);

    std::string playTime = formatTimeText(data.playTimeSeconds);
    renderTextCentered(playTime, CENTER_X, PLAY_TIME_Y, TIME_COLOR);
}

void GameOverRenderer::renderControlButtons() {
    renderTextCentered(TEXT_RESTART, CENTER_X, RESTART_BUTTON_Y, RESTART_COLOR);
    renderTextCentered(TEXT_QUIT, CENTER_X, QUIT_BUTTON_Y, QUIT_COLOR);
}

void GameOverRenderer::renderBackground() {
    SDL_Texture* bgTexture = assets->getTexture(AssetConfig::TEXTURE_GAME_OVER_BG);

    const auto renderTextureBackground = [&]() {
        SDL_Rect fullScreen{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &fullScreen);
    };

    const auto renderFallbackBackground = [&]() {
        SDL_SetRenderDrawColor(renderer, FALLBACK_BG_COLOR.r, FALLBACK_BG_COLOR.g,
                               FALLBACK_BG_COLOR.b, FALLBACK_BG_COLOR.a);
        SDL_RenderClear(renderer);
    };

    bgTexture && (renderTextureBackground(), true) || (renderFallbackBackground(), true);
}

void GameOverRenderer::renderTextCentered(const std::string& text, int x, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont(AssetConfig::FONT_GAME);

    font && (createTextTexture(text, color, font, x, y), true);
}

void GameOverRenderer::createTextTexture(const std::string& text, SDL_Color color, TTF_Font* font, int x, int y) {
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);

    const auto processTextSurface = [&]() {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        const auto renderAndCleanup = [&]() {
            renderTextTexture(texture, x, y, surface->w, surface->h);
            SDL_DestroyTexture(texture);
        };

        texture && (renderAndCleanup(), true);
        SDL_FreeSurface(surface);
    };

    surface && (processTextSurface(), true);
}

void GameOverRenderer::renderTextTexture(SDL_Texture* texture, int x, int y, int width, int height) {
    SDL_Rect rect{x - width / 2, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void GameOverRenderer::handleInput(InputKey key) {
    (key == InputKey::RESTART || key == InputKey::QUIT) &&
        (SoundManager::playImmediate(AssetConfig::SOUND_MENU_SELECT), true);
}

std::string GameOverRenderer::formatScoreText(int score) {
    return TEXT_FINAL_SCORE + std::to_string(score) + TEXT_SCORE_UNIT;
}

std::string GameOverRenderer::formatComboText(int combo) {
    return TEXT_MAX_COMBO + std::to_string(combo);
}

std::string GameOverRenderer::formatTimeText(int seconds) {
    return TEXT_PLAY_TIME + buildGameTimeDisplay(seconds);
}

std::string GameOverRenderer::buildGameTimeDisplay(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int remainingSeconds = totalSeconds % 60;
    return std::to_string(minutes) + TEXT_MINUTE + std::to_string(remainingSeconds) + TEXT_SECOND;
}
