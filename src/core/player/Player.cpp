#include "Player.h"
#include "../../ui/InputHandler.h"
#include "../game/GameConfig.h"

Player::Player()
    : x(GameConfig::PLAYER_START_X),
      y(GameConfig::MAP_GROUND_Y),
      movement(x, y),
      collision(x, y, movement, action) {}

void Player::handleInput(InputKey key) {
    if (collision.isDamaged()) return;

    action.handleInput(key);

    if (movement.handleInput(key)) {
        action.setActionCooldown(15);
    }
}

void Player::update() {
    action.update();
    movement.update();
    collision.update();
}

CollisionResult Player::processCollision(Building& building) {
    return collision.processCollision(building);
}

bool Player::isJumping() const {
    return movement.isJumping();
}

int Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

PlayerActionType Player::getAction() const {
    return action.getType();
}

bool Player::isDamaged() const {
    return collision.isDamaged();
}
