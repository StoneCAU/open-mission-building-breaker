#include "BuildingManager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

#include "../game/GameConfig.h"

BuildingManager::BuildingManager() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void BuildingManager::initBuildings() {
    buildings.clear();
    while (buildings.size() < GameConfig::MIN_ONSCREEN_BUILDINGS) {
        addRandomBuilding();
    }
    spawnCooldown = GameConfig::BUILDING_SPAWN_COOLDOWN;
}

void BuildingManager::updateAll() {
    applyPhysicsAll();  // 물리 먼저 적용
    handleSpawn();
    removeOffscreenBuildings();
}

void BuildingManager::applyPhysicsAll() {
    for (auto& b : buildings) {
        if (!b.isDestroyed()) {
            b.applyPhysics();
        }
    }
}

void BuildingManager::removeOffscreenBuildings() {
    buildings.erase(
        std::remove_if(buildings.begin(), buildings.end(),
            [](const Building& b) {
                return b.getGroundFrames() > 60 || b.getTopY() > GameConfig::MAP_GROUND_Y + 10;
            }),
        buildings.end()
    );
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

void BuildingManager::addRandomBuilding() {
    constexpr int MAX_ATTEMPTS = 50;
    int attempts = 0;
    while (attempts++ < MAX_ATTEMPTS) {
        int height = GameConfig::MIN_BUILDING_HEIGHT +
                     (std::rand() % (GameConfig::MAX_BUILDING_HEIGHT - GameConfig::MIN_BUILDING_HEIGHT + 1));
        int x = std::rand() % (GameConfig::MAP_WIDTH - GameConfig::BUILDING_WIDTH + 1);
        float y = static_cast<float>(-height);
        if (isOverlapping(x)) continue;
        buildings.emplace_back(x, y, height);
        break;
    }
}

bool BuildingManager::isOverlapping(int newX) const {
    for (const auto& b : buildings) {
        int leftA = newX;
        int rightA = newX + GameConfig::BUILDING_WIDTH - 1;
        int leftB = b.getX();
        int rightB = b.getX() + GameConfig::BUILDING_WIDTH - 1;
        if (rightA < leftB || leftA > rightB) continue;
        return true;
    }
    return false;
}

Building* BuildingManager::getBuildingAt(int x, float y) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        // 빌딩 범위 체크 (x 범위 + y 범위)
        bool xInRange = (x >= b.getX() && x < b.getX() + GameConfig::BUILDING_WIDTH);
        bool yInRange = (y >= b.getBottomY() && y <= b.getTopY());

        if (xInRange && yInRange) {
            return &b;
        }
    }
    return nullptr;
}

Building* BuildingManager::getBuildingInRange(int x, float y, float range) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        // X 범위 체크
        bool xInRange = (x >= b.getX() && x < b.getX() + GameConfig::BUILDING_WIDTH);

        // Y 범위 체크 (y 기준으로 range 내)
        bool yInRange = (b.getBottomY() >= y - range && b.getBottomY() <= y + range);

        if (xInRange && yInRange) {
            return &b;
        }
    }
    return nullptr;
}

Building* BuildingManager::getBuildingAbovePlayer(int x, float y, float threshold) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        // X 범위
        bool xInRange = (x >= b.getX() && x < b.getX() + GameConfig::BUILDING_WIDTH);

        // 빌딩 밑면이 플레이어 y 바로 위
        float buildingBottom = b.getBottomY();
        bool nearBottom = (buildingBottom < y && buildingBottom >= y - threshold);

        if (xInRange && nearBottom) {
            return &b;
        }
    }
    return nullptr;
}

void BuildingManager::destroyAll() {
    buildings.clear();
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
