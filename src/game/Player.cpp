#include "Player.h"

#include "GameConfig.h"

Player::Player()
    : x(GameConfig::MAP_WIDTH / 2), y(GameConfig::MAP_GROUND_Y),
      jumping(false), jumpFrame(0) {}

void Player::handleInput(InputKey key) {
    if (key == InputKey::LEFT && canMoveLeft()) --x;
    if (key == InputKey::RIGHT && canMoveRight()) ++x;
    if (key == InputKey::JUMP && !jumping) jump();
}

bool Player::canMoveLeft() const {
    return x > GameConfig::MAP_MIN_X;
}

bool Player::canMoveRight() const {
    return x < GameConfig::MAP_MAX_X;
}

void Player::jump() {
    jumping = true;
    jumpFrame = GameConfig::PLAYER_JUMP_DURATION;
    y = GameConfig::MAP_GROUND_Y - GameConfig::PLAYER_JUMP_DURATION;
}

void Player::update() {
    if (jumping) {
        --jumpFrame;
        if (jumpFrame <= 0) {
            jumping = false;
            y = GameConfig::MAP_GROUND_Y;
        }
    }
}

bool Player::isJumping() const {
    return jumping;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}
