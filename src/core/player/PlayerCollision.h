#pragma once
#include "CollisionResult.h"

class Building;
class PlayerMovement;
class PlayerAction;

class PlayerCollision {
public:
    PlayerCollision(int& x, float& y, 
                   PlayerMovement& movement, 
                   PlayerAction& action);

    CollisionResult processCollision(Building& building);
    
    void takeDamage();
    bool isInvincible() const;
    bool isDamaged() const;
    void update();

private:
    int& x;
    float& y;
    PlayerMovement& movement;
    PlayerAction& action;
    
    bool damaged;
    int damageFrame;
    
    CollisionResult tryHandleHeadCollision(Building& b);
    CollisionResult tryHandleAttackRange(Building& b);
    CollisionResult tryHandleDefenseRange(Building& b);
    CollisionResult handleBodyCollision(Building& b);
    
    CollisionResult processHeadDamage(Building& b);
    CollisionResult processAirCollision(Building& b);
    CollisionResult processGroundCollision(Building& b);
    
    bool isGroundLevel(float y) const;
};
