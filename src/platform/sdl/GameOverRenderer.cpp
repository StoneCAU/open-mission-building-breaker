#include "GameOverRenderer.h"

#include "SoundManager.h"
#include "../../core/game/GameOverDisplayData.h"

GameOverRenderer::GameOverRenderer(SDL_Renderer* r, AssetManager* a)
    : renderer(r), assets(a) {}

void GameOverRenderer::render(const GameOverDisplayData& data) {
    SoundManager::playBGM("gameover");

    SDL_Texture* bgTexture = assets->getTexture("game_over_bg");

    const auto renderBackground = [&]() {
        SDL_Rect fullScreen{0, 0, 800, 600};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &fullScreen);
    };

    const auto renderFallbackBackground = [&]() {
        SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
        SDL_RenderClear(renderer);
    };

    bgTexture && (renderBackground(), true) || (renderFallbackBackground(), true);

    renderTextCentered("게임 오버", 400, 120, {255, 100, 100, 255}); // 180→120

    std::string finalScore = "최종 점수: " + std::to_string(data.finalScore) + "점";
    renderTextCentered(finalScore, 400, 220, {255, 255, 255, 255}); // 280→220

    std::string maxCombo = "최고 콤보: x" + std::to_string(data.maxCombo);
    renderTextCentered(maxCombo, 400, 250, {255, 255, 100, 255}); // 310→250

    std::string playTime = "플레이 시간: " + buildGameTimeDisplay(data.playTimeSeconds);
    renderTextCentered(playTime, 400, 280, {200, 200, 200, 255}); // 340→280

    if (data.isNewRecord) {
        renderTextCentered("새로운 최고 기록!", 400, 320, {255, 255, 100, 255}); // 380→320
    }

    renderTextCentered("[R] 재시작", 400, 450, {100, 255, 100, 255});
    renderTextCentered("[Q] 종료", 400, 480, {255, 100, 100, 255});

    SoundManager::nextFrame();
    SDL_RenderPresent(renderer);
}

void GameOverRenderer::renderTextCentered(const std::string& text, int x, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont("game");

    const auto createAndRenderText = [&]() {
        SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);

        const auto renderSurface = [&]() {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            const auto copyTexture = [&]() {
                SDL_Rect rect{x - surface->w / 2, y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
                SDL_DestroyTexture(texture);
            };

            texture && (copyTexture(), true);
            SDL_FreeSurface(surface);
        };

        surface && (renderSurface(), true);
    };

    font && (createAndRenderText(), true);
}

std::string GameOverRenderer::buildGameTimeDisplay(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return std::to_string(minutes) + "분 " + std::to_string(seconds) + "초";
}
