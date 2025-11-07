#pragma once
#include "../ui/InputHandler.h"

class Player {
public:
    Player();

    void handleInput(InputKey key);
    void update();

    int getX() const;
    int getY() const;
    bool isJumping() const;

private:
    void jump();
    void applyJumpMotion();

    bool canMoveLeft() const;
    bool canMoveRight() const;

    int x;
    int y;
    bool jumping;
    bool canJump;
    int jumpFrame;
    int jumpCooldown;
};
