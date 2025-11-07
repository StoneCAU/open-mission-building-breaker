#pragma once
#include <vector>
#include "Building.h"

class BuildingManager {
public:
    BuildingManager();

    // ===== 초기화 및 업데이트 =====
    void initBuildings();
    void updateAll();

    // ===== 데이터 접근 =====
    std::vector<Building>& getAll();
    const std::vector<Building>& getAll() const;

private:
    std::vector<Building> buildings;
    int spawnCooldown = 0;

    void updateBuildings();
    void removeOffscreenBuildings();
    void handleSpawn();

    // ===== 내부 헬퍼 =====
    void addRandomBuilding();
    bool isOverlapping(int newX) const;
};
