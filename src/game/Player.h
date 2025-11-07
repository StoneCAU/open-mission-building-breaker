#pragma once
#include "Building.h"
#include "../ui/InputHandler.h"

enum class PlayerAction {
    IDLE,
    ATTACK,
    DEFEND,
    JUMP
};

class Player {
public:
    Player();

    void handleInput(InputKey key);
    void update();
    void forceFall(float newY);

    int getX() const;
    float getY() const;
    bool isJumping() const;

    // ====== 피격 여부 ======
    void takeDamage();
    bool isDamaged() const;
    bool isInvincible() const;
    void onReboundCollision(const Building& b, bool isGround);

    PlayerAction getAction() const;

private:
    // ====== 입력 처리 ======
    void handleGroundInput(InputKey key);
    void handleAirInput(InputKey key);
    void handleMovement(InputKey key);

    // ====== 액션 처리 ======
    bool tryAction(InputKey key, bool& canDo, PlayerAction type);
    bool tryJump(InputKey key);

    // ====== 점프 처리 ======
    void jump();
    void applyJumpMotion();
    void applyJumpRise(float half);
    void applyJumpFall(float half);
    void finishJump();

    // ====== 프레임 처리 ======
    void updateActionFrame();
    void updateKeyRelease();

    // ====== 이동 가능 여부 ======
    bool canMoveLeft() const;
    bool canMoveRight() const;

    int x;
    float y;

    bool jumping;
    bool canJump;
    bool canAttack;
    bool canDefend;
    bool damaged;
    bool hitHead;

    float jumpFrame;
    int jumpCooldown;
    int damageFrame;

    PlayerAction action;
    int actionFrame;
};