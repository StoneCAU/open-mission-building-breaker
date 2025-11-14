#include "PlayerMovement.h"
#include <windows.h>
#include "../../ui/InputHandler.h"
#include "../game/GameConfig.h"

PlayerMovement::PlayerMovement(int& x, float& y)
    : x(x), y(y), velocityY(0.0f),
      jumping(false), canJump(true), jumpCooldown(0) {}

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
    updateKeyRelease();
}

void PlayerMovement::applyPhysics() {
    // 중력과 속도 적용
    velocityY += GameConfig::GRAVITY;
    y += velocityY;

    // 지면 체크
    if (y >= GameConfig::MAP_GROUND_Y) {
        y = static_cast<float>(GameConfig::MAP_GROUND_Y);
        velocityY = 0.0f;
        jumping = false;
    }
}

void PlayerMovement::stopVerticalMovement() {
    velocityY = 0.0f;
}

void PlayerMovement::handleCollisionWith(float obstacleY) {
    // 위로 올라가는 중 장애물에 부딪혔을 때
    if (velocityY < 0) {
        // 속도 0으로 (더 이상 올라가지 못함)
        velocityY = 0.0f;
        // 위치는 장애물 바로 아래로
        y = obstacleY + GameConfig::PLAYER_HEIGHT;
    }
}

bool PlayerMovement::isOnGround() const {
    return y >= GameConfig::MAP_GROUND_Y - 0.1f;
}

void PlayerMovement::jump() {
    jumping = true;
    canJump = false;
    velocityY = GameConfig::JUMP_VELOCITY;
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
    const bool jumpKey =
        (key == InputKey::JUMP ||
         key == InputKey::MOVE_LEFT_JUMP ||
         key == InputKey::MOVE_RIGHT_JUMP);

    if (jumpKey && canJump && !jumping && jumpCooldown == 0) {
        jump();
        jumpCooldown = GameConfig::PLAYER_JUMP_COOLDOWN_MAX;
        return true;
    }
    return false;
}

void PlayerMovement::updateJumpCooldown() {
    if (jumpCooldown > 0) {
        --jumpCooldown;
    }
}

void PlayerMovement::updateKeyRelease() {
    if (!jumping && InputHandler::isKeyReleased(VK_UP)) {
        canJump = true;
    }
}

bool PlayerMovement::canMoveLeft() const {
    return x > GameConfig::MAP_MIN_X;
}

bool PlayerMovement::canMoveRight() const {
    return x < GameConfig::MAP_MAX_X;
}

bool PlayerMovement::isJumping() const {
    return jumping;
}

float PlayerMovement::getVelocityY() const {
    return velocityY;
}
