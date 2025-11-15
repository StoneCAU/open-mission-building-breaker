#include "PlayerCollision.h"

PlayerCollision::PlayerCollision()
    : invincibilityFrame(0) {}

void PlayerCollision::reset() {
    invincibilityFrame = 0;
}

void PlayerCollision::update() {
    if (invincibilityFrame > 0) {
        --invincibilityFrame;
    }
}

void PlayerCollision::takeDamage() {
    if (invincibilityFrame > 0) {
        return;
    }
    
    invincibilityFrame = INVINCIBILITY_FRAMES;
}

bool PlayerCollision::isDamaged() const {
    return invincibilityFrame > 0;
}
