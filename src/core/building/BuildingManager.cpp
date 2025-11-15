#include "BuildingManager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "../game/GameConfig.h"

namespace {
    constexpr int SPAWN_X_SPACING = 2;
    constexpr int OFFSCREEN_BOTTOM_THRESHOLD = 3;
    constexpr int OFFSCREEN_TOP_THRESHOLD = -10;
}

BuildingManager::BuildingManager() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    nextSpawnX = 0;
}

void BuildingManager::initBuildings() {
    buildings.clear();
    while (buildings.size() < GameConfig::MIN_ONSCREEN_BUILDINGS) {
        addRandomBuilding();
    }
    spawnCooldown = GameConfig::BUILDING_SPAWN_COOLDOWN;
}

void BuildingManager::updateAll() {
    applyPhysicsAll();
    removeOffscreenBuildings();
    handleSpawn();
}

void BuildingManager::destroyAll() {
    buildings.clear();
}

void BuildingManager::addRandomBuilding() {
    int height = GameConfig::MIN_BUILDING_HEIGHT +
                 (std::rand() % (GameConfig::MAX_BUILDING_HEIGHT - GameConfig::MIN_BUILDING_HEIGHT + 1));

    int x = nextSpawnX;
    nextSpawnX += GameConfig::BUILDING_WIDTH + SPAWN_X_SPACING;

    if (nextSpawnX + GameConfig::BUILDING_WIDTH > GameConfig::MAP_WIDTH) {
        nextSpawnX = 0;
    }

    float y = static_cast<float>(-height);
    buildings.emplace_back(x, y, height);
}

Building* BuildingManager::getBuildingAt(int x, float y) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        if (!isXInRange(b, x)) continue;

        if (isYContains(b, y)) {
            return &b;
        }
    }
    return nullptr;
}

Building* BuildingManager::getBuildingInRange(int x, float y, float range) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        if (!isXInRange(b, x)) continue;

        if (isYInRange(b.getBottomY(), y, range)) {
            return &b;
        }
    }
    return nullptr;
}

Building* BuildingManager::getBuildingAbove(int x, float y, float range) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        if (!isXInRange(b, x)) continue;

        if (isYAboveTarget(b.getBottomY(), y, range)) {
            return &b;
        }
    }
    return nullptr;
}

Building* BuildingManager::getBuildingAbovePlayer(int x, float y, float threshold) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        if (!isXInRange(b, x)) continue;

        if (isYAboveTarget(b.getBottomY(), y, threshold)) {
            return &b;
        }
    }
    return nullptr;
}

int BuildingManager::getActiveCount() const {
    int count = 0;
    for (const auto& b : buildings) {
        if (!b.isDestroyed()) {
            ++count;
        }
    }
    return count;
}

std::vector<Building>& BuildingManager::getAll() {
    return buildings;
}

const std::vector<Building>& BuildingManager::getAll() const {
    return buildings;
}

void BuildingManager::applyPhysicsAll() {
    for (auto& b : buildings) {
        if (!b.isDestroyed()) {
            b.applyPhysics();
        }
    }
}

void BuildingManager::removeOffscreenBuildings() {
    auto it = std::remove_if(buildings.begin(), buildings.end(),
        [this](const Building& b) {
            return shouldRemoveBuilding(b);
        });

    buildings.erase(it, buildings.end());
}

void BuildingManager::handleSpawn() {
    if (spawnCooldown > 0) {
        --spawnCooldown;
        return;
    }

    if (buildings.size() < GameConfig::MAX_ONSCREEN_BUILDINGS) {
        addRandomBuilding();
    }

    spawnCooldown = GameConfig::BUILDING_SPAWN_COOLDOWN;
}

bool BuildingManager::shouldRemoveBuilding(const Building& b) {
    if (b.isDestroyed()) {
        return true;
    }

    bool tooFarDown = b.getBottomY() > GameConfig::MAP_GROUND_Y + OFFSCREEN_BOTTOM_THRESHOLD;
    bool tooFarUp = b.getTopY() < OFFSCREEN_TOP_THRESHOLD && !b.isRebounded();

    return tooFarDown || tooFarUp;
}

bool BuildingManager::isXInRange(const Building& b, int x) {
    return x >= b.getX() && x < b.getX() + GameConfig::BUILDING_WIDTH;
}

bool BuildingManager::isYInRange(float buildingBottom, float y, float range) {
    return buildingBottom >= y - range && buildingBottom <= y + range;
}

bool BuildingManager::isYAboveTarget(float buildingBottom, float targetY, float range) {
    return buildingBottom >= targetY && buildingBottom <= targetY + range;
}

bool BuildingManager::isYContains(const Building& b, float y) const {
    return y >= b.getBottomY() && y <= b.getTopY();
}
