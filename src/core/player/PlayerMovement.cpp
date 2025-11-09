#include "PlayerMovement.h"
#include <windows.h>

#include "../../ui/InputHandler.h"
#include "../game/GameConfig.h"

namespace {
    inline bool isKeyReleased(int key) {
        return (GetAsyncKeyState(key) & 0x8000) == 0;
    }
}

PlayerMovement::PlayerMovement(int& x, float& y)
    : x(x), y(y),
      jumping(false),
      canJump(true),
      jumpFrame(0),
      jumpCooldown(0) {}

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
        return true;
    }
    return false;
}

void PlayerMovement::jump() {
    jumping = true;
    canJump = false;
    jumpFrame = 0;
}

void PlayerMovement::applyJumpMotion() {
    const float half = GameConfig::PLAYER_JUMP_DURATION / 2.0f;

    if (jumpFrame < half) {
        applyJumpRise(half);
        return;
    }

    if (jumpFrame < GameConfig::PLAYER_JUMP_DURATION) {
        applyJumpFall(half);
        return;
    }

    finishJump();
}

void PlayerMovement::applyJumpRise(float half) {
    const float t = jumpFrame / half;
    y = GameConfig::MAP_GROUND_Y - (GameConfig::PLAYER_JUMP_HEIGHT * t);
    ++jumpFrame;
}

void PlayerMovement::applyJumpFall(float half) {
    const float t = (jumpFrame - half) / half;
    y = (GameConfig::MAP_GROUND_Y - GameConfig::PLAYER_JUMP_HEIGHT)
      + (GameConfig::PLAYER_JUMP_HEIGHT * t);
    ++jumpFrame;
}

void PlayerMovement::finishJump() {
    y = GameConfig::MAP_GROUND_Y;
    jumping = false;
    jumpCooldown = GameConfig::PLAYER_JUMP_COOLDOWN_MAX;
}

void PlayerMovement::forceFall(float newY) {
    y = newY;

    if (jumping) {
        jumpFrame = GameConfig::PLAYER_JUMP_DURATION / 2.0f;
    }

    if (y >= GameConfig::MAP_GROUND_Y - 0.1f) {
        finishJump();
    }
}

void PlayerMovement::update() {
    if (jumping) {
        applyJumpMotion();
    }

    updateJumpCooldown();
    updateKeyRelease();
}

void PlayerMovement::updateJumpCooldown() {
    if (jumpCooldown > 0) {
        --jumpCooldown;
    }
}

void PlayerMovement::updateKeyRelease() {
    if (!jumping && isKeyReleased(VK_UP)) {
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
