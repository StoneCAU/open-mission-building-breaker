#pragma once

#include <SDL2/SDL.h>
#include <vector>

struct PixelSprite {
    std::vector<std::vector<int>> data;
    int width;
    int height;
    SDL_Color baseColor;
};

class PixelSpriteRenderer {
public:
    PixelSpriteRenderer(SDL_Renderer* renderer);

    void initializeSprites();
    void renderSprite(const PixelSprite& sprite, int x, int y, int scale);
    const PixelSprite& getPlayerSprite() const;
    const PixelSprite& getAttackSprite() const;
    const PixelSprite& getDamagedSprite() const;
    const PixelSprite& getBuildingSprite() const;

private:
    SDL_Renderer* renderer;
    PixelSprite playerSprite;
    PixelSprite playerAttackSprite;
    PixelSprite playerDamagedSprite;
    PixelSprite buildingSprite;

    void renderRect(int x, int y, int w, int h, SDL_Color color);
    SDL_Color getPixelColor(const PixelSprite& sprite, int pixelType);
};
