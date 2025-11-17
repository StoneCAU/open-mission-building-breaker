#include "MenuRenderer.h"

#include <iostream>

#include "SoundManager.h"

MenuRenderer::MenuRenderer(SDL_Renderer* r, AssetManager* a) 
    : renderer(r), assets(a) {}

void MenuRenderer::render(int highScore) {
    SoundManager::playBGM("menu");

    renderBackground();
    renderTitle();
    renderButtons();

    SoundManager::nextFrame();
    SDL_RenderPresent(renderer);
}

void MenuRenderer::handleInput(InputKey key) {
    (key == InputKey::ENTER || key == InputKey::QUIT) &&
        (SoundManager::playImmediate("menu_select"), true);
}

void MenuRenderer::renderBackground() {
    SDL_Texture* bgTexture = assets->getTexture("dojo_bg");
    if (bgTexture) {
        SDL_Rect fullScreen = {0, 0, 800, 600};
        SDL_RenderCopy(renderer, bgTexture, nullptr, &fullScreen);
    } else {
        // Fallback 배경
        SDL_SetRenderDrawColor(renderer, 26, 26, 26, 255);
        SDL_RenderClear(renderer);
    }
}

void MenuRenderer::renderTitle() {
    SDL_Texture* titleTexture = assets->getTexture("title");
    if (!titleTexture) return;
    
    int originalW, originalH;
    SDL_QueryTexture(titleTexture, nullptr, nullptr, &originalW, &originalH);

    int titleW = (originalW * 5) / 10;
    int titleH = (originalH * 5) / 10;

    int titleX = (800 - titleW) / 2;
    int titleY = 10;

    SDL_Rect titleRect = {titleX, titleY, titleW, titleH};
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
}

void MenuRenderer::renderScoreBox(int highScore) {
    int boxX = 250, boxY = 300, boxW = 300, boxH = 60;

    renderRect(boxX, boxY, boxW, boxH, {45, 27, 20, 180});

    SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
    SDL_Rect border = {boxX - 2, boxY - 2, boxW + 4, boxH + 4};
    SDL_RenderDrawRect(renderer, &border);

    renderTextCentered("HIGH SCORE", 400, boxY + 12, "menu", {218, 165, 32, 255});
    std::string scoreText = std::to_string(highScore) + " PTS";
    renderTextCentered(scoreText, 400, boxY + 35, "menu", {245, 245, 220, 255});
}

void MenuRenderer::renderButtons() {
    renderTextCentered("PRESS START", 400, 450, "menu", {139, 0, 0, 255});
    renderTextCentered("Q - QUIT", 400, 480, "menu", {150, 150, 150, 255});
    renderTextCentered("WOOWA TECH COURSE 8TH - OPEN MISSION", 400, 540, "menu", {120, 120, 120, 255});
    renderTextCentered("MADE BY HONG SEOKWOO", 400, 565, "menu", {110, 110, 110, 255});
}

void MenuRenderer::renderTextCentered(const std::string& text, int x, int y, const std::string& fontName, SDL_Color color) {
    TTF_Font* font = assets->getFont(fontName);
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

void MenuRenderer::renderRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}
