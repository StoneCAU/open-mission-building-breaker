#include "BuildingManager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

#include "../game/GameConfig.h"

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

void BuildingManager::applyPhysicsAll() {
    for (auto& b : buildings) {
        if (!b.isDestroyed()) {
            b.applyPhysics();
        }
    }
}

void BuildingManager::removeOffscreenBuildings() {
    auto it = std::remove_if(buildings.begin(), buildings.end(),
        [](const Building& b) {
            // 파괴된 빌딩 제거
            if (b.isDestroyed()) {
                return true;
            }

            // 화면 아래로 충분히 내려가면 제거 (바닥 관통)
            bool tooFarDown = b.getBottomY() > GameConfig::MAP_GROUND_Y + 3;

            // 화면 위로 올라갔는데, rebound 아니면 제거
            bool tooFarUp = b.getTopY() < -10 && !b.isRebounded();

            return tooFarDown || tooFarUp;
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

void BuildingManager::addRandomBuilding() {
    int height = GameConfig::MIN_BUILDING_HEIGHT +
                 (std::rand() % (GameConfig::MAX_BUILDING_HEIGHT - GameConfig::MIN_BUILDING_HEIGHT + 1));

    // X 좌표를 순차적으로 배치 (겹침 절대 없음)
    int x = nextSpawnX;
    nextSpawnX += GameConfig::BUILDING_WIDTH + 2;  // 간격 2칸

    // 화면 끝 넘으면 처음으로
    if (nextSpawnX + GameConfig::BUILDING_WIDTH > GameConfig::MAP_WIDTH) {
        nextSpawnX = 0;
    }

    float y = static_cast<float>(-height);
    buildings.emplace_back(x, y, height);
}

bool BuildingManager::isOverlapping(int newX) const {
    for (const auto& b : buildings) {
        // 화면 밖 위쪽 빌딩은 겹침 체크 안 함
        if (b.getY() < 0) {
            continue;
        }

        int leftA = newX;
        int rightA = newX + GameConfig::BUILDING_WIDTH - 1;
        int leftB = b.getX();
        int rightB = b.getX() + GameConfig::BUILDING_WIDTH - 1;

        if (rightA < leftB || leftA > rightB) {
            continue;
        }

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

Building* BuildingManager::getBuildingAbove(int x, float y, float range) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        // X 범위 체크
        bool xInRange = (x >= b.getX() && x < b.getX() + GameConfig::BUILDING_WIDTH);

        // Y 범위: 플레이어 위쪽 range 범위 안에 빌딩 밑면이 있으면
        float buildingBottom = b.getBottomY();
        bool yInRange = (buildingBottom >= y && buildingBottom <= y + range);

        if (xInRange && yInRange) {
            return &b;
        }
    }
    return nullptr;
}

Building* BuildingManager::getBuildingAbovePlayer(int x, float y, float threshold) {
    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        bool xInRange = (x >= b.getX() && x < b.getX() + GameConfig::BUILDING_WIDTH);

        float buildingBottom = b.getBottomY();

        // 빌딩 밑면이 플레이어 머리보다 위에 있고 + threshold 이내
        bool nearBottom = (buildingBottom >= y && buildingBottom <= y + threshold);

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
