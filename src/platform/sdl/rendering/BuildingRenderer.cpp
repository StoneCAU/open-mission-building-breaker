#include "BuildingRenderer.h"
#include "../../../core/building/Building.h"
#include "../assets/AssetConfig.h"
#include "../assets/AssetManager.h"

BuildingRenderer::BuildingRenderer(SDL_Renderer* renderer, AssetManager* assets)
    : renderer(renderer), assets(assets) {}

void BuildingRenderer::renderBuilding(const Building& building, int screenX, int screenY) {
    int height = building.getHeight();

    for (int floor = 0; floor < height; ++floor) {
        int floorScreenY = screenY - (floor * FLOOR_HEIGHT);
        renderFloor(building, floor, screenX, floorScreenY);
    }
}

void BuildingRenderer::renderFloor(const Building& building, int floorIndex, int screenX, int screenY) {
    std::string spriteName = determineFloorSprite(building, floorIndex);
    SDL_Texture* texture = assets->getTexture(spriteName);

    const auto renderFloorTexture = [&]() {
        int spriteWidth = calculateSpriteWidth(building);
        SDL_Rect destRect{screenX, screenY, spriteWidth, FLOOR_HEIGHT};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    };

    texture && (renderFloorTexture(), true);
}

std::string BuildingRenderer::determineFloorSprite(const Building& building, int floorIndex) const {
    const auto renderLines = building.getRenderLines();

    const auto checkDestructionState = [&]() {
        return floorIndex < renderLines.size() &&
               hasDestructionMarker(renderLines[floorIndex]);
    };

    const auto checkTopFloor = [&]() {
        return isTopFloor(building, floorIndex);
    };

    std::string result = AssetConfig::TEXTURE_FLOOR_NORMAL;

    checkDestructionState() && (result = AssetConfig::TEXTURE_FLOOR_DESTRUCTION, true);
    checkTopFloor() && !checkDestructionState() && (result = AssetConfig::TEXTURE_BUILDING_TOP, true);

    return result;
}

bool BuildingRenderer::hasDestructionMarker(const std::string& floorVisual) const {
    return floorVisual.find(DESTRUCTION_MARKER) != std::string::npos;
}

bool BuildingRenderer::isTopFloor(const Building& building, int floorIndex) const {
    return floorIndex == building.getHeight() - 1;
}

int BuildingRenderer::calculateSpriteWidth(const Building& building) const {
    return Building::WIDTH * PIXELS_PER_WIDTH_UNIT;
}
