#include "Player.h"
#include "../game/GameConfig.h"

Player::Player()
    : x(GameConfig::PLAYER_START_X),
      y(static_cast<float>(GameConfig::MAP_GROUND_Y)),
      movement(x, y),
      collision(x, y, movement, action) {}

void Player::handleInput(InputKey key) {
    if (collision.isDamaged()) return;

    action.handleInput(key);

    if (movement.handleInput(key)) {
        action.setActionCooldown(GameConfig::PLAYER_JUMP_COOLDOWN_MAX);
    }
}

void Player::update() {
    action.update();
    movement.update();
    collision.update();
}

void Player::takeDamage() {
    collision.takeDamage();
}

int Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

float Player::getVelocityY() const {
    return movement.getVelocityY();
}

PlayerActionType Player::getAction() const {
    return action.getType();
}

bool Player::isDamaged() const {
    return collision.isDamaged();
}

bool Player::isAttackActiveFrame() const {
    return action.isAttackActiveFrame();
}

void Player::handlePhysicsCollision(float obstacleY) {
    movement.handleCollisionWith(obstacleY);
}
