#pragma once
#include <SDL2/SDL.h>
#include <string>

class Building;
class AssetManager;

class BuildingRenderer {
public:
    BuildingRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void renderBuilding(const Building& building, int screenX, int screenY);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    static constexpr int PIXELS_PER_WIDTH_UNIT = 20;
    static constexpr int FLOOR_HEIGHT = 20;
    static constexpr char DESTRUCTION_MARKER = '*';

    void renderFloor(const Building& building, int floorIndex, int screenX, int screenY);
    std::string determineFloorSprite(const Building& building, int floorIndex) const;
    bool hasDestructionMarker(const std::string& floorVisual) const;
    bool isTopFloor(const Building& building, int floorIndex) const;
    int calculateSpriteWidth(const Building& building) const;
};
