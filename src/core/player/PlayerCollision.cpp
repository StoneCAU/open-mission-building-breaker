#include "PlayerCollision.h"
#include "../game/GameConfig.h"

PlayerCollision::PlayerCollision(int& x, float& y,
                                 PlayerMovement& movement,
                                 PlayerAction& action)
    : x(x), y(y),
      movement(movement),
      action(action),
      damaged(false),
      damageFrame(0) {}

void PlayerCollision::reset() {
    damaged = false;
    damageFrame = 0;
}

void PlayerCollision::update() {
    if (damageFrame > 0) {
        --damageFrame;
        if (damageFrame == 0) {
            damaged = false;
        }
    }
}

void PlayerCollision::takeDamage() {
    if (damageFrame > 0) return;
    damaged = true;
    damageFrame = GameConfig::PLAYER_INVINCIBILITY_FRAMES;
}

bool PlayerCollision::isDamaged() const {
    return damaged;
}

bool PlayerCollision::isInvincible() const {
    return damageFrame > 0;
}
