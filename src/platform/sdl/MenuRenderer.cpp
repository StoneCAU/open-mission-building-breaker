#include "MenuRenderer.h"
#include "AssetConfig.h"
#include "SoundManager.h"

MenuRenderer::MenuRenderer(SDL_Renderer* r, AssetManager* a)
    : renderer(r), assets(a) {}

void MenuRenderer::render(int highScore) {
    initializeFrame();
    renderMenuContent(highScore);
    finalizeFrame();
}

void MenuRenderer::initializeFrame() {
    SoundManager::playBGM(AssetConfig::MUSIC_MENU);
}

void MenuRenderer::renderMenuContent(int highScore) {
    renderBackground();
    renderTitle();
    renderButtons();
}

void MenuRenderer::finalizeFrame() {
    SoundManager::nextFrame();
    SDL_RenderPresent(renderer);
}

void MenuRenderer::handleInput(InputKey key) {
    (key == InputKey::ENTER || key == InputKey::QUIT) &&
        (SoundManager::playImmediate(AssetConfig::SOUND_MENU_SELECT), true);
}

void MenuRenderer::renderBackground() {
    SDL_Texture* bgTexture = assets->getTexture(AssetConfig::TEXTURE_DOJO_BG);

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

void MenuRenderer::renderTitle() {
    SDL_Texture* titleTexture = assets->getTexture(AssetConfig::TEXTURE_TITLE);

    const auto renderTitleTexture = [&]() {
        int originalW, originalH;
        SDL_QueryTexture(titleTexture, nullptr, nullptr, &originalW, &originalH);

        SDL_Rect titleRect = calculateTitleRect(originalW, originalH);
        SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    };

    titleTexture && (renderTitleTexture(), true);
}

void MenuRenderer::renderScoreBox(int highScore) {
    renderScoreBoxBackground();
    renderScoreBoxBorder();
    renderScoreContent(highScore);
}

void MenuRenderer::renderScoreBoxBackground() {
    SDL_Rect boxRect = calculateScoreBoxRect();
    renderRect(boxRect.x, boxRect.y, boxRect.w, boxRect.h, SCORE_BOX_COLOR);
}

void MenuRenderer::renderScoreBoxBorder() {
    SDL_Rect borderRect = calculateScoreBorderRect();
    SDL_SetRenderDrawColor(renderer, SCORE_BORDER_COLOR.r, SCORE_BORDER_COLOR.g,
                           SCORE_BORDER_COLOR.b, SCORE_BORDER_COLOR.a);
    SDL_RenderDrawRect(renderer, &borderRect);
}

void MenuRenderer::renderScoreContent(int highScore) {
    renderTextCentered(TEXT_HIGH_SCORE, CENTER_X, SCORE_BOX_Y + SCORE_TITLE_Y_OFFSET,
                       AssetConfig::FONT_MENU, SCORE_TITLE_COLOR);

    std::string scoreText = std::to_string(highScore) + TEXT_SCORE_SUFFIX;
    renderTextCentered(scoreText, CENTER_X, SCORE_BOX_Y + SCORE_TEXT_Y_OFFSET,
                       AssetConfig::FONT_MENU, SCORE_TEXT_COLOR);
}

void MenuRenderer::renderButtons() {
    renderTextCentered(TEXT_START, CENTER_X, START_BUTTON_Y, AssetConfig::FONT_MENU, START_BUTTON_COLOR);
    renderTextCentered(TEXT_QUIT, CENTER_X, QUIT_BUTTON_Y, AssetConfig::FONT_MENU, QUIT_BUTTON_COLOR);
    renderTextCentered(TEXT_CREDIT1, CENTER_X, CREDIT_LINE1_Y, AssetConfig::FONT_MENU, CREDIT_COLOR1);
    renderTextCentered(TEXT_CREDIT2, CENTER_X, CREDIT_LINE2_Y, AssetConfig::FONT_MENU, CREDIT_COLOR2);
}

void MenuRenderer::renderTextCentered(const std::string& text, int x, int y, const std::string& fontName, SDL_Color color) {
    SDL_Surface* surface = createTextSurface(text, fontName, color);

    const auto processSurface = [&]() {
        SDL_Texture* texture = createTextTexture(surface);

        const auto renderAndCleanup = [&]() {
            renderTextTexture(texture, surface, x, y);
            cleanupTextResources(surface, texture);
        };

        texture && (renderAndCleanup(), true);
    };

    surface && (processSurface(), true);
}

SDL_Surface* MenuRenderer::createTextSurface(const std::string& text, const std::string& fontName, SDL_Color color) {
    TTF_Font* font = assets->getFont(fontName);
    SDL_Surface* result = nullptr;

    font && (result = TTF_RenderUTF8_Solid(font, text.c_str(), color), true);

    return result;
}

SDL_Texture* MenuRenderer::createTextTexture(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
}

void MenuRenderer::renderTextTexture(SDL_Texture* texture, SDL_Surface* surface, int x, int y) {
    SDL_Rect rect{x - surface->w / 2, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void MenuRenderer::cleanupTextResources(SDL_Surface* surface, SDL_Texture* texture) {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

SDL_Rect MenuRenderer::calculateTitleRect(int originalW, int originalH) {
    int titleW = (originalW * TITLE_SCALE_NUMERATOR) / TITLE_SCALE_DENOMINATOR;
    int titleH = (originalH * TITLE_SCALE_NUMERATOR) / TITLE_SCALE_DENOMINATOR;
    int titleX = (SCREEN_WIDTH - titleW) / 2;

    return {titleX, TITLE_Y, titleW, titleH};
}

SDL_Rect MenuRenderer::calculateScoreBoxRect() {
    return {SCORE_BOX_X, SCORE_BOX_Y, SCORE_BOX_WIDTH, SCORE_BOX_HEIGHT};
}

SDL_Rect MenuRenderer::calculateScoreBorderRect() {
    return {SCORE_BOX_X - SCORE_BOX_BORDER_SIZE, SCORE_BOX_Y - SCORE_BOX_BORDER_SIZE,
            SCORE_BOX_WIDTH + SCORE_BOX_BORDER_SIZE * 2, SCORE_BOX_HEIGHT + SCORE_BOX_BORDER_SIZE * 2};
}

void MenuRenderer::renderRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect{x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}
