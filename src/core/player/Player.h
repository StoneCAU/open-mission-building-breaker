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

    bool isJumping() const;
    int getX() const;
    float getY() const;
    PlayerActionType getAction() const;
    bool isDamaged() const;

private:
    int x;
    float y;

    PlayerMovement movement;
    PlayerAction action;
    PlayerCollision collision;
};
