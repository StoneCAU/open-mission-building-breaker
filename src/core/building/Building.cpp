#include "Building.h"

#include "../game/GameConfig.h"


constexpr const char* BLOCK_UNIT = "#";

Building::Building(int x, int y, int height)
    : x(x),
      y(y),
      yPos(static_cast<float>(y)),
      height(height),
      destroyed(false),
      falling(true),
      rebounding(false),
      reboundPower(0) {
    initShape();
}

/**
 * height층만큼 "▩▩▩" 형태로 구성
 */
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

/**
 * 기본 하강 (매 프레임 BUILDING_FALL_SPEED 만큼 하강)
 */
void Building::updateFall() {
    if (destroyed || rebounding) {
        return;
    }

    yPos += GameConfig::BUILDING_FALL_SPEED;
    y = static_cast<int>(yPos);
}

/**
 * 하단 한 층 제거
 */
void Building::takeHit() {
    if (destroyed || shape.empty()) {
        return;
    }

    shape.pop_back();
    --height;
    --yPos;
    y = static_cast<int>(yPos);

    if (shape.empty()) {
        destroyed = true;
    }
}

/**
 * 방어 성공 시 튕겨올라감 (즉시 호출)
 */
void Building::rebound() {
    if (destroyed) {
        return;
    }
    reboundPower = GameConfig::BUILDING_REBOUND_STRENGTH;
    falling = false;
    rebounding = true;
}

/**
 * 위로 튕기는 중 한 프레임
 */
void Building::updateRebound() {
    if (!rebounding) {
        return;
    }

    yPos -= 1.0f;
    y = static_cast<int>(yPos);
    reboundPower -= GameConfig::BUILDING_REBOUND_DECAY;

    if (reboundPower <= 0) {
        rebounding = false;
        falling = true;
    }
}

bool Building::collidesWith(int playerX, float playerY) const {
    // 플레이어와 수평 위치가 겹치는가?
    bool xOverlap = (playerX >= x && playerX < x + GameConfig::BUILDING_WIDTH);

    // 플레이어가 빌딩의 높이 범위 안에 있는가?
    int buildingTop = y - height + 1;
    bool yOverlap = (playerY <= y && playerY >= buildingTop);

    return xOverlap && yOverlap;
}

/**
 * 상태 조회
 */
bool Building::isDestroyed() const { return destroyed; }
bool Building::isRebounding() const { return rebounding; }
bool Building::isFalling() const { return falling; }

/**
 * 좌표 / 정보 조회
 */
int Building::getX() const { return x; }
int Building::getY() const { return y; }
int Building::getHeight() const { return height; }
std::vector<std::string> Building::getRenderLines() const { return shape; }
