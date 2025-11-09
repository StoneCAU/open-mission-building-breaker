#pragma once
#include <vector>
#include "Building.h"

class BuildingManager {
public:
    BuildingManager();

    void initBuildings();
    void updateAll();

    Building* getBuildingAt(int x, float y);
    std::vector<Building>& getAll();
    const std::vector<Building>& getAll() const;

private:
    std::vector<Building> buildings;
    int spawnCooldown = 0;

    void updateBuildings();
    void removeOffscreenBuildings();
    void handleSpawn();
    void addRandomBuilding();
    bool isOverlapping(int newX) const;
};
