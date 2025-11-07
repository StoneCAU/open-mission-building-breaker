#pragma once
#include "../ui/InputHandler.h"

class Player {
public:
    Player();

    void handleInput(InputKey key);
    void jump();
    void update();

    int getX() const;
    int getY() const;
    bool isJumping() const;

private:
    bool canMoveLeft() const;
    bool canMoveRight() const;

    int x;
    int y;
    bool jumping;
    int jumpFrame;
};
