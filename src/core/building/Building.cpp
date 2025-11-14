#include "Building.h"
#include "../game/GameConfig.h"

Building::Building(int x, float y, int height)
    : x(x), y(y), velocityY(0.0f), destroyed(false), groundFrames(0) {

    floors.reserve(height);
    for (int i = 0; i < height; ++i) {
        floors.emplace_back(FloorType::NORMAL);
    }
}

void Building::applyPhysics() {
    if (destroyed) {
        return;
    }

    // 이미 지면이면 정지
    if (isOnGround()) {
        velocityY = 0.0f;
        ++groundFrames;
        return;
    }

    // 중력 적용
    velocityY += GameConfig::BUILDING_GRAVITY;
    y += velocityY;

    // 지면 체크
    if (y >= GameConfig::MAP_GROUND_Y) {
        y = static_cast<float>(GameConfig::MAP_GROUND_Y);
        velocityY = 0.0f;
    }
}

void Building::applyRebound() {
    velocityY = -1.5f;
}

void Building::stopVerticalMovement() {
    velocityY = 0.0f;
}

bool Building::isOnGround() const {
    return y >= GameConfig::MAP_GROUND_Y - 0.1f;
}

void Building::removeBottomFloor() {
    if (floors.empty()) {
        destroyed = true;
        return;
    }

    floors.erase(floors.begin());

    if (floors.empty()) {
        destroyed = true;
    }
}

Floor* Building::getFloorAt(float worldY) {
    if (worldY < y || worldY > getTopY()) {
        return nullptr;
    }

    int floorIndex = static_cast<int>(getTopY() - worldY);

    if (floorIndex < 0 || floorIndex >= static_cast<int>(floors.size())) {
        return nullptr;
    }

    return &floors[floorIndex];
}

int Building::getHeight() const {
    return static_cast<int>(floors.size());
}

bool Building::isDestroyed() const {
    return destroyed;
}

std::vector<std::string> Building::getRenderLines() const {
    std::vector<std::string> lines;
    lines.reserve(floors.size());

    for (const auto& floor : floors) {
        if (!floor.isDestroyed()) {
            lines.push_back(floor.getVisual());
        }
    }

    return lines;
}

int Building::getX() const {
    return x;
}

float Building::getY() const {
    return y;
}

float Building::getBottomY() const {
    return y;
}

float Building::getTopY() const {
    return y + static_cast<float>(floors.size());
}

int Building::getGroundFrames() const {
    return groundFrames;
}

float Building::getVelocityY() const {
    return velocityY;
}
