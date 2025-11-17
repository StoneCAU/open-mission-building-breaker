#include "BuildingRenderer.h"
#include "../../core/building/Building.h"
#include "AssetManager.h"

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
    std::string spriteName = getFloorSpriteName(building, floorIndex);
    SDL_Texture* texture = assets->getTexture(spriteName);

    const auto renderFloorTexture = [&]() {
        int spriteWidth = Building::WIDTH * PIXELS_PER_WIDTH_UNIT;
        SDL_Rect destRect{screenX, screenY, spriteWidth, FLOOR_HEIGHT};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    };

    texture && (renderFloorTexture(), true);
}

std::string BuildingRenderer::getFloorSpriteName(const Building& building, int floorIndex) const {
    const auto renderLines = building.getRenderLines();

    if (floorIndex < renderLines.size()) {
        const std::string& floorVisual = renderLines[floorIndex];
        if (floorVisual.find('*') != std::string::npos) {
            return "floor_destruction";
        }
    }

    if (floorIndex == building.getHeight() - 1) {
        return "building_top";
    }

    return "floor_normal";
}

bool BuildingRenderer::isTopFloor(const Building& building, int floorIndex) const {
    return floorIndex == building.getHeight() - 1;
}

bool BuildingRenderer::isFloorDestroyed(const Building& building, int floorIndex) const {
    const auto renderLines = building.getRenderLines();
    if (floorIndex < renderLines.size()) {
        return renderLines[floorIndex].find('*') != std::string::npos;
    }
    return false;
}
