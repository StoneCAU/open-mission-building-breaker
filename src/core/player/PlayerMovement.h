#pragma once
#include "../../ui/InputHandler.h"

class PlayerMovement {
public:
    PlayerMovement(int& x, float& y);

    void update();
    
    void forceFall(float newY);
    bool isJumping() const;
    bool handleInput(InputKey key);

private:
    int& x;
    float& y;
    
    bool jumping;
    bool canJump;
    int jumpFrame;
    int jumpCooldown;
    
    void jump();
    void applyJumpMotion();
    void applyJumpRise(float half);
    void applyJumpFall(float half);
    void finishJump();
    
    void updateJumpCooldown();
    void updateKeyRelease();
    
    bool canMoveLeft() const;
    bool canMoveRight() const;

    void handleMovement(InputKey key);
    bool tryJump(InputKey key);
};
