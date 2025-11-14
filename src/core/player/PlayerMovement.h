#pragma once
#include "../../ui/InputHandler.h"

class PlayerMovement {
public:
    PlayerMovement(int& x, float& y);

    bool handleInput(InputKey key);
    void update();

    // 물리
    void applyPhysics();
    void stopVerticalMovement();
    bool isOnGround() const;

    // 점프
    void jump();
    bool isJumping() const;

    float getVelocityY() const;
    void setY(float newY);

private:
    int& x;
    float& y;
    float velocityY;
    bool jumping;
    bool canJump;
    int jumpCooldown;

    void handleMovement(InputKey key);
    bool tryJump(InputKey key);
    void updateJumpCooldown();
    void updateKeyRelease();
    bool canMoveLeft() const;
    bool canMoveRight() const;
};
