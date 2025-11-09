#pragma once
#include <vector>
#include <string>

class Building {
public:
    Building(int x, int y, int height);

    // ====== 상태 갱신 ======
    void updateFall();
    void updateRebound();
    void takeHit();

    // ====== 상태 전이 ======
    void rebound();

    // ====== 상태 조회 ======
    bool isDestroyed() const;
    bool isRebounding() const;
    bool isFalling() const;
    bool collidesWith(int playerX, float playerY) const;

    // ====== 좌표 및 렌더 정보 ======
    int getX() const;
    int getY() const;
    int getHeight() const;

    std::vector<std::string> getRenderLines() const;

private:
    // ----- 위치 / 크기 -----
    int x; // 콘솔 상의 가로 위치
    int y; // 현재 바닥 Y좌표
    float yPos;
    int height; // 현재 층 수

    // ----- 상태 -----
    bool destroyed;
    bool falling; // 기본 true (항상 하강)
    bool rebounding; // 방어 성공 시 true
    int reboundPower; // 튕김 남은 프레임 수

    // ----- 시각적 데이터 -----
    std::vector<std::string> shape;

    // ----- 내부 -----
    void initShape();
};
