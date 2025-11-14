#pragma once
#include <vector>
#include "Building.h"

class BuildingManager {
public:
    BuildingManager();

    void initBuildings();
    void updateAll();
    void destroyAll();
    void applyPhysicsAll();
    int getActiveCount() const;

    Building* getBuildingAt(int x, float y);
    Building* getBuildingInRange(int x, float y, float range);
    Building* getBuildingAbovePlayer(int x, float y, float threshold);
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
