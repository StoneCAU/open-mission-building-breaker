#pragma once
#include <SDL2/SDL.h>
#include <string>

class Building;
class Floor;
class AssetManager;

class BuildingRenderer {
public:
    BuildingRenderer(SDL_Renderer* renderer, AssetManager* assets);
    
    void renderBuilding(const Building& building, int screenX, int screenY);
    
private:
    SDL_Renderer* renderer;
    AssetManager* assets;
    
    static constexpr int PIXELS_PER_WIDTH_UNIT = 8;
    static constexpr int FLOOR_HEIGHT = 12;
    
    void renderFloor(const Building& building, int floorIndex, int screenX, int screenY);
    std::string getFloorSpriteName(const Building& building, int floorIndex) const;
    bool isTopFloor(const Building& building, int floorIndex) const;
    bool isFloorDestroyed(const Building& building, int floorIndex) const;
};
