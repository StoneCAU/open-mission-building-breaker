#pragma once
#include "CollisionResult.h"
#include "../ui/InputHandler.h"

class Building;

enum class PlayerAction {
    IDLE,
    ATTACK,
    DEFEND
};


class Player {
public:
    Player();

    void handleInput(InputKey key);
    void update();
    CollisionResult processCollision(Building& building);

    bool isJumping() const;
    int getX() const;
    float getY() const;
    PlayerAction getAction() const;
    bool isDamaged() const;

    void takeDamage();
    bool isInvincible() const;
    void forceFall(float newY);

private:
    int x;
    float y;
    bool jumping;
    bool canJump;
    bool canAttack;
    bool canDefend;
    int jumpFrame;
    int jumpCooldown;
    PlayerAction action;
    int actionFrame;
    bool damaged;
    int damageFrame;
    bool hitHead;
    int actionCooldown;

    void handleGroundInput(InputKey key);
    void handleAirInput(InputKey key);
    void handleMovement(InputKey key);
    bool tryAction(InputKey key, bool& canDo, PlayerAction type);
    bool tryJump(InputKey key);

    void jump();
    void applyJumpMotion();
    void applyJumpRise(float half);
    void applyJumpFall(float half);
    void finishJump();

    void updateActionFrame();
    void updateKeyRelease();

    bool canMoveLeft() const;
    bool canMoveRight() const;

    CollisionResult tryHandleHeadCollision(Building& b);
    CollisionResult tryHandleAttackRange(Building& b);
    CollisionResult tryHandleDefenseRange(Building& b);
    CollisionResult handleBodyCollision(Building& b);

    CollisionResult processHeadDamage(Building& b);
    CollisionResult processAirCollision(Building& b);
    CollisionResult processGroundCollision(Building& b);

    bool isGroundLevel(float y) const;
};
