#pragma once
#include "../../ui/InputHandler.h"

class PlayerMovement {
public:
    PlayerMovement(int& x, float& y);

    void handleMovement(InputKey key);
    bool tryJump(InputKey key);
    void update();
    
    void forceFall(float newY);
    bool isJumping() const;

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
};
