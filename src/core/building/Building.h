#pragma once
#include <vector>
#include <string>

#include "Floor.h"

class Building {
public:
    Building(int x, float y, int height);

    void applyPhysics();
    void applyRebound();
    void stopVerticalMovement();
    bool isOnGround() const;

    void removeBottomFloor();
    Floor* getFloorAt(float worldY);
    int getHeight() const;
    bool isDestroyed() const;

    std::vector<std::string> getRenderLines() const;

    int getX() const;
    float getY() const;
    float getBottomY() const;
    float getTopY() const;

private:
    int x;
    float y;
    float velocityY;
    std::vector<Floor> floors;
    bool destroyed;

    std::vector<std::string> shape;

    void initShape();
};
