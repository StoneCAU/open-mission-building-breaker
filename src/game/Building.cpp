#include "Building.h"

constexpr const char* BLOCK_UNIT = "▩";
constexpr int BUILDING_WIDTH = 3;
constexpr int MIN_BUILDING_HEIGHT = 3;
constexpr int MAX_BUILDING_HEIGHT = 6;
constexpr int REBOUND_STRENGTH = 3;  // 튕겨오르는 세기
constexpr int REBOUND_DECAY = 1;     // 프레임당 감속량

Building::Building(int x, int startY, int height)
    : x(x),
      y(startY),
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
 * 기본 하강(매 프레임 y++)
 */
void Building::updateFall() {
    if (destroyed || rebounding) return; // rebounding 중이면 낙하 정지
    ++y;
}

/**
 * 하단 한 층 제거
 */
void Building::takeHit() {
    if (destroyed || shape.empty()) return;

    shape.pop_back();
    --height;

    if (shape.empty()) {
        destroyed = true;
    }
}

/**
 * 방어 성공 시 튕겨올라감 (즉시 호출)
 */
void Building::rebound() {
    if (destroyed) return;
    reboundPower = REBOUND_STRENGTH;
    falling = false;
    rebounding = true;
}

/**
 * 위로 튕기는 중 한 프레임
 */
void Building::updateRebound() {
    if (!rebounding) return;

    y -= 1;
    reboundPower -= REBOUND_DECAY;

    if (reboundPower <= 0) {
        rebounding = false;
        falling = true;
    }
}

/**
 * 상태 조회
 */
bool Building::isDestroyed() const {
    return destroyed;
}

bool Building::isRebounding() const {
    return rebounding;
}
bool Building::isFalling() const {
    return falling;
}

/**
 * 좌표 / 정보 조회
 */
int Building::getX() const {
    return x;
}

int Building::getY() const {
    return y;
}

int Building::getHeight() const {
    return height;
}

std::vector<std::string> Building::getRenderLines() const {
    return shape;
}
