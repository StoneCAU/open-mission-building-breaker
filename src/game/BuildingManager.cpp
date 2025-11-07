#include "BuildingManager.h"
#include "GameConfig.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

BuildingManager::BuildingManager() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void BuildingManager::initBuildings() {
    buildings.clear();
    while (buildings.size() < GameConfig::MIN_ONSCREEN_BUILDINGS) {
        addRandomBuilding();
    }
}

void BuildingManager::updateAll() {
    // ===== 상태 갱신 =====
    for (auto& b : buildings) {
        if (b.isRebounding()) b.updateRebound();
        else b.updateFall();
    }

    // ===== 화면 아래로 완전히 사라진 빌딩 제거 =====
    buildings.erase(
        std::remove_if(buildings.begin(), buildings.end(),
            [](const Building& b) {
                return b.getY() - b.getHeight() > GameConfig::MAP_GROUND_Y + 1;
            }),
        buildings.end()
    );

    // ===== 부족하면 새 빌딩 생성 =====
    while (buildings.size() < GameConfig::MIN_ONSCREEN_BUILDINGS) {
        addRandomBuilding();
    }
}

void BuildingManager::addRandomBuilding() {
    constexpr int MAX_ATTEMPTS = 50;
    int attempts = 0;

    while (attempts++ < MAX_ATTEMPTS) {
        int height = GameConfig::MIN_BUILDING_HEIGHT +
                     (std::rand() % (GameConfig::MAX_BUILDING_HEIGHT - GameConfig::MIN_BUILDING_HEIGHT + 1));
        int x = std::rand() % (GameConfig::MAP_WIDTH - GameConfig::BUILDING_WIDTH + 1);
        int y = -height;

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
        if (!(rightA < leftB || leftA > rightB)) {
            return true;
        }
    }
    return false;
}

std::vector<Building>& BuildingManager::getAll() {
    return buildings;
}

const std::vector<Building>& BuildingManager::getAll() const {
    return buildings;
}
