#pragma once

#include "PlayerMovement.h"
#include "PlayerAction.h"
#include "PlayerCollision.h"
#include "PlayerAttachment.h"
#include "../../ui/InputHandler.h"

class Building;

class Player {
public:
    static constexpr float HEIGHT = 1.0f;
    static constexpr float ATTACK_RANGE = 2.0f;
    static constexpr float DEFENSE_RANGE = 0.5f;
    static constexpr int START_X = 25;

    Player();

    void reset();
    void handleInput(InputKey key);
    void update();
    void takeDamage();

    int getX() const;
    float getY() const;
    float getVelocityY() const;
    PlayerActionType getAction() const;
    bool isDamaged() const;
    bool isAttackActiveFrame() const;
    bool isJumping() const;

    void handlePhysicsCollision(float obstacleY);
    void attachToBuilding(Building* building);
    void detachFromBuilding();
    bool isAttachedToBuilding() const;

private:
    static constexpr int JUMP_ACTION_COOLDOWN = 5;

    int x;
    float y;

    PlayerMovement movement;
    PlayerAction action;
    PlayerCollision collision;
    PlayerAttachment attachment;

    void updateAttachedState();
    void updateComponents();
    void handleJumpInput(InputKey key);
    void handleActionInput(InputKey key);

    bool shouldDetachFromBuilding(Building* building) const;
    void handleBuildingDetachment(Building* building);
    void followAttachedBuilding(Building* building);
};
