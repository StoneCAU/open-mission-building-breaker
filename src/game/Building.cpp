#include "Building.h"
#include "../game/GameConfig.h" // GameConfig::BUILDING_FALL_SPEED 사용

constexpr const char* BLOCK_UNIT = "▩";
constexpr int BUILDING_WIDTH = 3;
constexpr int MIN_BUILDING_HEIGHT = 3;
constexpr int MAX_BUILDING_HEIGHT = 6;
constexpr int REBOUND_STRENGTH = 3;
constexpr int REBOUND_DECAY = 1;

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
    for (int i = 0; i < BUILDING_WIDTH; ++i) {
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
    reboundPower = REBOUND_STRENGTH;
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
    reboundPower -= REBOUND_DECAY;

    if (reboundPower <= 0) {
        rebounding = false;
        falling = true;
    }
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
