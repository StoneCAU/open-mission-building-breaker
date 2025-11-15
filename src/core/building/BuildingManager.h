#pragma once
#include <vector>
#include "Building.h"

class BuildingManager {
public:
    BuildingManager();

    void initBuildings();
    void updateAll();
    void destroyAll();

    void addRandomBuilding();

    Building* getBuildingAt(int x, float y);
    Building* getBuildingInRange(int x, float y, float range);
    Building* getBuildingAbove(int x, float y, float range);
    Building* getBuildingAbovePlayer(int x, float y, float threshold);

    int getActiveCount() const;
    std::vector<Building>& getAll();
    const std::vector<Building>& getAll() const;

private:
    std::vector<Building> buildings;
    int spawnCooldown{};
    int nextSpawnX;

    void applyPhysicsAll();
    void removeOffscreenBuildings();
    void handleSpawn();

    bool shouldRemoveBuilding(const Building& b);
    bool isXInRange(const Building& b, int x);
    bool isYInRange(float buildingBottom, float y, float range);
    bool isYAboveTarget(float buildingBottom, float targetY, float range);
    bool isYContains(const Building& b, float y) const;
};
