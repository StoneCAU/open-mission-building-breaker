#include "PlayerMovement.h"
#include <windows.h>

#include "Player.h"
#include "../../ui/InputHandler.h"
#include "../game/GameConfig.h"

PlayerMovement::PlayerMovement(int& x, float& y)
    : x(x), 
      y(y), 
      velocityY(0.0f),
      jumping(false), 
      canJump(true), 
      jumpCooldown(0),
      startX(Player::START_X),
      groundY(static_cast<float>(GameConfig::MAP_GROUND_Y)),
      mapMinX(GameConfig::MAP_MIN_X),
      mapMaxX(GameConfig::MAP_MAX_X) {}

void PlayerMovement::reset() {
    x = startX;
    y = groundY;
    velocityY = 0.0f;
    jumping = false;
    canJump = true;
    jumpCooldown = 0;
}

bool PlayerMovement::handleInput(InputKey key) {
    bool jumped = tryJump(key);

    if (!jumping) {
        handleMovement(key);
    }

    return jumped;
}

void PlayerMovement::update() {
    applyPhysics();
    updateJumpCooldown();
    updateJumpRelease();
}

void PlayerMovement::applyPhysics() {
    velocityY += GRAVITY;
    y += velocityY;

    if (y >= groundY) {
        y = groundY;
        velocityY = 0.0f;
        jumping = false;
    }
}

void PlayerMovement::stopVerticalMovement() {
    velocityY = 0.0f;
}

void PlayerMovement::handleCollisionWith(float obstacleY, float playerHeight) {
    if (velocityY < UPWARD_VELOCITY_THRESHOLD) {
        velocityY = 0.0f;
        y = obstacleY + playerHeight;
    }
}

bool PlayerMovement::isOnGround() const {
    return y >= groundY - GROUND_CHECK_THRESHOLD;
}

void PlayerMovement::jump() {
    jumping = true;
    canJump = false;
    velocityY = JUMP_VELOCITY;
}

void PlayerMovement::handleMovement(InputKey key) {
    if (key == InputKey::LEFT && canMoveLeft()) {
        --x;
    }
    if (key == InputKey::RIGHT && canMoveRight()) {
        ++x;
    }
}

bool PlayerMovement::tryJump(InputKey key) {
    if (!isJumpKey(key)) {
        return false;
    }

    if (!canStartJump()) {
        return false;
    }

    jump();
    jumpCooldown = JUMP_COOLDOWN_MAX;
    return true;
}

bool PlayerMovement::isJumpKey(InputKey key) const {
    return key == InputKey::JUMP ||
           key == InputKey::MOVE_LEFT_JUMP ||
           key == InputKey::MOVE_RIGHT_JUMP;
}

bool PlayerMovement::canStartJump() const {
    return canJump && !jumping && jumpCooldown == 0;
}

void PlayerMovement::followObject(float targetY, float targetVelocityY) {
    y = targetY;
    velocityY = targetVelocityY;
    jumping = false;
}

void PlayerMovement::updateJumpCooldown() {
    if (jumpCooldown > 0) {
        --jumpCooldown;
    }
}

void PlayerMovement::updateJumpRelease() {
    if (!jumping && isJumpKeyReleased()) {
        canJump = true;
    }
}

bool PlayerMovement::isJumpKeyReleased() const {
    return InputHandler::isKeyReleased(VK_UP);
}

bool PlayerMovement::canMoveLeft() const {
    return x > mapMinX;
}

bool PlayerMovement::canMoveRight() const {
    return x < mapMaxX;
}

bool PlayerMovement::isJumping() const {
    return jumping;
}

float PlayerMovement::getVelocityY() const {
    return velocityY;
}
