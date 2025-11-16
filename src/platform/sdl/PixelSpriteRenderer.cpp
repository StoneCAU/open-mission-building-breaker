#include "PixelSpriteRenderer.h"

PixelSpriteRenderer::PixelSpriteRenderer(SDL_Renderer* r) : renderer(r) {}

void PixelSpriteRenderer::initializeSprites() {
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
}

SDL_Color PixelSpriteRenderer::getPixelColor(const PixelSprite& sprite, int pixelType) {
    if (pixelType == 1) return sprite.baseColor;
    if (pixelType == 2) return {220, 220, 220, 255};
    if (pixelType == 3) return {100, 100, 100, 255};
    if (pixelType == 4) return {255, 255, 100, 255};
    return sprite.baseColor;
}

void PixelSpriteRenderer::renderSprite(const PixelSprite& sprite, int x, int y, int scale) {
    for (int row = 0; row < sprite.height; ++row) {
        for (int col = 0; col < sprite.width; ++col) {
            int pixelType = sprite.data[row][col];
            if (pixelType == 0) continue;

            SDL_Color color = getPixelColor(sprite, pixelType);
            renderRect(x + col * scale, y + row * scale, scale, scale, color);
        }
    }
}

void PixelSpriteRenderer::renderRect(int x, int y, int w, int h, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

const PixelSprite& PixelSpriteRenderer::getPlayerSprite() const { return playerSprite; }
const PixelSprite& PixelSpriteRenderer::getAttackSprite() const { return playerAttackSprite; }
const PixelSprite& PixelSpriteRenderer::getDamagedSprite() const { return playerDamagedSprite; }
const PixelSprite& PixelSpriteRenderer::getBuildingSprite() const { return buildingSprite; }
