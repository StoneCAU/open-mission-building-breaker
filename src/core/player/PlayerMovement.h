#pragma once

#include "../../ui/InputHandler.h"

class PlayerMovement {
public:
    PlayerMovement(int& x, float& y);

    void reset();
    bool handleInput(InputKey key);
    void update();

    void applyPhysics();
    void handleCollisionWith(float obstacleY, float playerHeight);
    void followObject(float targetY, float targetVelocityY);
    void stopVerticalMovement();

    bool isOnGround() const;
    bool isJumping() const;
    float getVelocityY() const;

private:
    static constexpr float GRAVITY = 0.1f;
    static constexpr float JUMP_VELOCITY = -1.0f;
    static constexpr int JUMP_DURATION = 20;
    static constexpr int JUMP_COOLDOWN_MAX = 15;
    static constexpr float GROUND_CHECK_THRESHOLD = 0.1f;
    static constexpr float UPWARD_VELOCITY_THRESHOLD = 0.0f;

    int& x;
    float& y;
    float velocityY;
    bool jumping;
    bool canJump;
    int jumpCooldown;

    int startX;
    float groundY;
    int mapMinX;
    int mapMaxX;

    void jump();
    bool tryJump(InputKey key);
    void handleMovement(InputKey key);
    void updateJumpCooldown();
    void updateJumpRelease();

    bool canMoveLeft() const;
    bool canMoveRight() const;
    bool isJumpKey(InputKey key) const;
    bool canStartJump() const;
    bool isJumpKeyReleased() const;
};
