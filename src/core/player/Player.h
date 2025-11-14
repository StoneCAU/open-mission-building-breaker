#pragma once
#include "PlayerMovement.h"
#include "PlayerAction.h"
#include "PlayerCollision.h"

enum class InputKey;
enum class PlayerActionType;
class Building;

class Player {
public:
    Player();

    void handleInput(InputKey key);
    void handlePhysicsCollision(float obstacleY);
    void takeDamage();
    void update();

    void attachToBuilding(Building* building);
    void detachFromBuilding();
    bool isAttachedToBuilding() const;

    // Getter
    int getX() const;
    float getY() const;
    float getVelocityY() const;
    PlayerActionType getAction() const;
    bool isDamaged() const;
    bool isJumping() const;
    bool isAttackActiveFrame() const;

private:
    int x;
    float y;
    PlayerMovement movement;
    PlayerAction action;
    PlayerCollision collision;
    Building* attachedBuilding;

    void onPlayerDamaged();
};