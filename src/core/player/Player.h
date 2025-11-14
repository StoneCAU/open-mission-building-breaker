#pragma once
#include "CollisionResult.h"
#include "PlayerMovement.h"
#include "PlayerAction.h"
#include "PlayerCollision.h"
#include "PlayerActionType.h"
#include "../../ui/InputHandler.h"

class Building;

class Player {
public:
    Player();

    void handleInput(InputKey key);
    void update();
    CollisionResult processCollision(Building& building);

    // Getters
    bool isJumping() const;
    int getX() const;
    float getY() const;
    float getVelocityY() const;
    PlayerActionType getAction() const;
    bool isDamaged() const;
    bool isStuckUnderBuilding() const;

private:
    int x;
    float y;

    PlayerMovement movement;
    PlayerAction action;
    PlayerCollision collision;

    bool stuckUnderBuilding;
    Building* stuckBuilding;

    void updateStuckState();
    bool shouldReleaseFromBuilding() const;
    void handleCollisionResult(const CollisionResult& result);
    void stickToBuilding(Building* building);
    void releaseFromBuilding();
};
