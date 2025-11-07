#pragma once
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

    int getX() const;
    int getY() const;
    bool isJumping() const;
    PlayerAction getAction() const;

private:
    void jump();
    void applyJumpMotion();

    bool canMoveLeft() const;
    bool canMoveRight() const;

    int x;
    int y;
    bool jumping;
    bool canJump;
    bool canAttack;
    bool canDefend;
    int jumpFrame;
    int jumpCooldown;
    int actionFrame;

    PlayerAction action;
};
