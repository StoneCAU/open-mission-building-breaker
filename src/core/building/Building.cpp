#include "Building.h"
#include "../game/GameConfig.h"
#include <algorithm>

Building::Building(int x, float y, int height)
    : x(x),
      y(y),
      velocityY(0.0f),
      destroyed(false),
      groundFrames(0),
      rebounded(false) {

    floors.reserve(height);
    for (int i = 0; i < height; ++i) {
        floors.emplace_back(FloorType::NORMAL);
    }
}

void Building::applyPhysics() {
    if (destroyed) {
        return;
    }

    applyGravity();
    updatePosition();
}

void Building::applyRebound() {
    velocityY = REBOUND_VELOCITY;
    rebounded = true;
}

void Building::updateFloors() {
    for (auto& floor : floors) {
        floor.update();
    }

    removeDestroyedFloors();
}

void Building::removeBottomFloor() {
    if (floors.empty()) {
        destroyed = true;
        return;
    }

    floors.front().takeDamage(1);
}

void Building::damageAllFloors() {
    for (auto& floor : floors) {
        floor.takeDamage(999);
    }
}

void Building::removeDestroyedFloors() {
    auto it = std::remove_if(floors.begin(), floors.end(),
        [](const Floor& f) {
            return f.shouldRemove();
        });

    floors.erase(it, floors.end());

    if (floors.empty()) {
        destroyed = true;
    }
}

bool Building::isDestroyed() const {
    return destroyed;
}

bool Building::isRebounded() const {
    return rebounded;
}

bool Building::isOnGround() const {
    return y >= GameConfig::MAP_GROUND_Y - 0.1f;
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

int Building::getHeight() const {
    return static_cast<int>(floors.size());
}

float Building::getVelocityY() const {
    return velocityY;
}

int Building::getGroundFrames() const {
    return groundFrames;
}

std::vector<std::string> Building::getRenderLines() const {
    std::vector<std::string> lines;
    lines.reserve(floors.size());

    for (const auto& floor : floors) {
        lines.push_back(floor.getVisual());
    }

    return lines;
}

void Building::applyGravity() {
    if (rebounded && velocityY < 0) {
        velocityY += REBOUND_GRAVITY;
        return;
    }

    velocityY += GRAVITY;

    if (rebounded && velocityY >= 0) {
        rebounded = false;
    }
}

void Building::updatePosition() {
    y += velocityY;
}
