#pragma once
#include "CollisionResult.h"

class Building;
class PlayerMovement;
class PlayerAction;
enum class PlayerActionType;

class PlayerCollision {
public:
    PlayerCollision(int& x, float& y, PlayerMovement& movement, PlayerAction& action);

    void reset();
    void update();
    void takeDamage();

    bool isDamaged() const;
    bool isInvincible() const;

private:
    int& x;
    float& y;

    PlayerMovement& movement;
    PlayerAction& action;

    bool damaged;
    int damageFrame;
};
