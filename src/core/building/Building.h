#pragma once
#include <vector>
#include <string>

#include "Floor.h"

class Building {
public:
    static constexpr int WIDTH = 6;
    static constexpr int MIN_HEIGHT = 5;
    static constexpr int MAX_HEIGHT = 8;

    Building(int x, float y, int height);

    void applyPhysics();
    void applyRebound();

    void removeBottomFloor();

    bool isDestroyed() const;
    bool isRebounded() const;
    bool isOnGround() const;

    int getX() const;
    float getY() const;
    float getBottomY() const;
    float getTopY() const;
    int getHeight() const;
    float getVelocityY() const;
    int getGroundFrames() const;

    std::vector<std::string> getRenderLines() const;

private:
    static constexpr float GRAVITY = 0.001f;
    static constexpr float REBOUND_VELOCITY = -0.7f;
    static constexpr float REBOUND_GRAVITY = 0.02f;

    int x;
    float y;
    float velocityY;
    bool destroyed;
    bool rebounded;
    int groundFrames;

    std::vector<Floor> floors;
    std::vector<std::string> shape;

    void applyGravity();
    void updatePosition();
};
