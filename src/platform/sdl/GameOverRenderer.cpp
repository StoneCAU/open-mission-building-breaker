#include "GameOverRenderer.h"
#include "../../core/game/GameOverDisplayData.h"

GameOverRenderer::GameOverRenderer(SDL_Renderer* r, AssetManager* a) 
    : renderer(r), assets(a) {}

void GameOverRenderer::render(const GameOverDisplayData& data) {
    SDL_SetRenderDrawColor(renderer, 25, 25, 40, 255);
    SDL_RenderClear(renderer);

    renderTextCentered("게임 오버", 400, 180, {255, 100, 100, 255});

    std::string finalScore = "최종 점수: " + std::to_string(data.finalScore) + "점";
    renderTextCentered(finalScore, 400, 280, {255, 255, 255, 255});

    std::string maxCombo = "최고 콤보: x" + std::to_string(data.maxCombo);
    renderTextCentered(maxCombo, 400, 310, {255, 255, 100, 255});

    std::string playTime = "플레이 시간: " + buildGameTimeDisplay(data.playTimeSeconds);
    renderTextCentered(playTime, 400, 340, {200, 200, 200, 255});

    if (data.isNewRecord) {
        renderTextCentered("새로운 최고 기록!", 400, 380, {255, 255, 100, 255});
    }

    renderTextCentered("[R] 재시작", 400, 450, {100, 255, 100, 255});
    renderTextCentered("[Q] 종료", 400, 480, {255, 100, 100, 255});

    SDL_RenderPresent(renderer);
}

void GameOverRenderer::renderTextCentered(const std::string& text, int x, int y, SDL_Color color) {
    TTF_Font* font = assets->getFont("game");
    if (!font) return;

    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture) {
        SDL_Rect rect = {x - surface->w / 2, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(surface);
}

std::string GameOverRenderer::buildGameTimeDisplay(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return std::to_string(minutes) + "분 " + std::to_string(seconds) + "초";
}
