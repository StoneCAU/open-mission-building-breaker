#include "Player.h"

#include <windows.h>

#include "GameConfig.h"

Player::Player()
    : x(GameConfig::PLAYER_START_X),
      y(GameConfig::MAP_GROUND_Y),
      jumping(false),
      canJump(true),
      jumpFrame(0),
      jumpCooldown(0) {
}

void Player::handleInput(InputKey key) {
    if (jumping) return;

    // 이동 중 점프 조합키
    if ((key == InputKey::MOVE_LEFT_JUMP || key == InputKey::MOVE_RIGHT_JUMP) &&
        canJump && !jumping && jumpCooldown == 0) {
        jump();
        return; // 이동 중단
        }

    // 일반 점프
    if (key == InputKey::JUMP && canJump && !jumping && jumpCooldown == 0) {
        jump();
        return;
    }

    // 이동 처리
    if (key == InputKey::LEFT && canMoveLeft())  --x;
    if (key == InputKey::RIGHT && canMoveRight()) ++x;
}

bool Player::canMoveLeft() const {
    return x > GameConfig::MAP_MIN_X;
}

bool Player::canMoveRight() const {
    return x < GameConfig::MAP_MAX_X;
}

void Player::jump() {
    jumping = true;
    canJump = false;
    jumpFrame = 0;
}

void Player::applyJumpMotion() {
    // 총 프레임을 절반으로 나눠 상승/하강 구간 구성
    const float half = GameConfig::PLAYER_JUMP_DURATION / 2.0f;

    if (jumpFrame < half) {
        // 상승: 0 -> half 동안 선형 상승
        const float t = (jumpFrame / half); // 0..1
        y = GameConfig::MAP_GROUND_Y - (GameConfig::PLAYER_JUMP_HEIGHT * t);
    } else {
        // 하강: half -> duration 동안 선형 하강
        const float t = (jumpFrame - half) / half; // 0..1
        y = (GameConfig::MAP_GROUND_Y - GameConfig::PLAYER_JUMP_HEIGHT) + (GameConfig::PLAYER_JUMP_HEIGHT * t);
    }

    ++jumpFrame;

    if (jumpFrame >= GameConfig::PLAYER_JUMP_DURATION) {
        // 착지
        y = GameConfig::MAP_GROUND_Y;
        jumping = false;
        jumpCooldown = GameConfig::PLAYER_JUMP_COOLDOWN_MAX;
    }
}

void Player::update() {
    if (jumping)
        applyJumpMotion();

    if (jumpCooldown > 0)
        --jumpCooldown;

    // ↑ 키가 올라간 순간에만 다시 점프 허용
    // (키를 계속 누르고 있으면 canJump는 false 유지)
    const bool upPressed = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;
    if (!jumping && !upPressed)
        canJump = true;
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
