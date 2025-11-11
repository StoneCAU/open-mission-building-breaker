#pragma once
#include <vector>
#include <string>

class Building {
public:
    Building(int x, int y, int height);

    void updateFall();
    void updateRebound();
    void takeHit();

    void rebound();

    bool isDestroyed() const;
    bool isRebounding() const;
    bool isFalling() const;
    bool collidesWith(int playerX, float playerY) const;

    int getX() const;
    int getY() const;
    int getHeight() const;

    std::vector<std::string> getRenderLines() const;

private:
    int x;
    int renderY;
    float physicsY;
    int height;

    bool destroyed;
    bool falling;
    bool rebounding;
    int reboundFramesLeft;

    std::vector<std::string> shape;

    void initShape();
};
