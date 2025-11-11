#include "Building.h"
#include "../game/GameConfig.h"

constexpr const char* BLOCK_UNIT = "#";

Building::Building(int x, int y, int height)
    : x(x),
      renderY(y),
      physicsY(static_cast<float>(y)),
      height(height),
      destroyed(false),
      falling(true),
      rebounding(false),
      reboundFramesLeft(0) {
    initShape();
}

void Building::initShape() {
    shape.clear();
    std::string blockLine;
    for (int i = 0; i < GameConfig::BUILDING_WIDTH; ++i) {
        blockLine += BLOCK_UNIT;
    }
    for (int i = 0; i < height; ++i) {
        shape.push_back(blockLine);
    }
}

void Building::updateFall() {
    if (destroyed || rebounding) {
        return;
    }

    // 부드러운 낙하를 위해 실수 좌표 업데이트
    physicsY += GameConfig::BUILDING_FALL_SPEED_PER_FRAME;

    // 렌더링용 정수 좌표 동기화
    renderY = static_cast<int>(physicsY);
}

void Building::takeHit() {
    if (destroyed || shape.empty()) {
        return;
    }

    shape.pop_back();
    --height;

    if (shape.empty()) {
        destroyed = true;
    }
}

void Building::rebound() {
    if (destroyed) {
        return;
    }
    reboundFramesLeft = GameConfig::BUILDING_REBOUND_DURATION_FRAMES;
    falling = false;
    rebounding = true;
}

void Building::updateRebound() {
    if (!rebounding) {
        return;
    }

    physicsY -= GameConfig::BUILDING_REBOUND_SPEED_PER_FRAME;
    renderY = static_cast<int>(physicsY);
    reboundFramesLeft -= GameConfig::BUILDING_REBOUND_DECAY_PER_FRAME;

    if (reboundFramesLeft <= 0) {
        rebounding = false;
        falling = true;
    }
}

bool Building::collidesWith(int playerX, float playerY) const {
    bool xOverlap = playerX >= x && playerX < x + GameConfig::BUILDING_WIDTH;

    int buildingTop = renderY - height + 1;
    bool yOverlap = playerY <= renderY && playerY >= buildingTop;

    return xOverlap && yOverlap;
}

bool Building::isDestroyed() const { return destroyed; }
bool Building::isRebounding() const { return rebounding; }
bool Building::isFalling() const { return falling; }

int Building::getX() const { return x; }
int Building::getY() const { return renderY; }
int Building::getHeight() const { return height; }
std::vector<std::string> Building::getRenderLines() const { return shape; }
