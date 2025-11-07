#include "Player.h"
#include "GameConfig.h"
#include <windows.h>

Player::Player()
    : x(GameConfig::PLAYER_START_X),
      y(GameConfig::MAP_GROUND_Y),
      jumping(false),
      canJump(true),
      canAttack(true),
      canDefend(true),
      jumpFrame(0),
      jumpCooldown(0),
      action(PlayerAction::IDLE),
      actionFrame(0) {}

void Player::handleInput(InputKey key) {
    // ===== 점프 중 행동 =====
    if (jumping) {
        if (key == InputKey::ATTACK && canAttack) {
            action = PlayerAction::ATTACK;
            actionFrame = GameConfig::PLAYER_ACTION_DURATION;
            canAttack = false;
            return;
        }
        if (key == InputKey::DEFEND && canDefend) {
            action = PlayerAction::DEFEND;
            actionFrame = GameConfig::PLAYER_ACTION_DURATION;
            canDefend = false;
            return;
        }
        return;
    }
    // ===== 평상시 행동 =====
    if (key == InputKey::ATTACK && canAttack) {
        action = PlayerAction::ATTACK;
        actionFrame = GameConfig::PLAYER_ACTION_DURATION;
        canAttack = false;
        return;
    }

    if (key == InputKey::DEFEND && canDefend) {
        action = PlayerAction::DEFEND;
        actionFrame = GameConfig::PLAYER_ACTION_DURATION;
        canDefend = false;
        return;
    }

    if ((key == InputKey::MOVE_LEFT_JUMP || key == InputKey::MOVE_RIGHT_JUMP) &&
        canJump && !jumping && jumpCooldown == 0) {
        jump();
        return;
        }

    if (key == InputKey::JUMP && canJump && !jumping && jumpCooldown == 0) {
        jump();
        return;
    }

    if (key == InputKey::LEFT && canMoveLeft()) {
        --x;
    }

    if (key == InputKey::RIGHT && canMoveRight()) {
        ++x;
    }

    action = PlayerAction::IDLE;
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
    const float half = GameConfig::PLAYER_JUMP_DURATION / 2.0f;

    if (jumpFrame < half) {
        const float t = jumpFrame / half;
        y = GameConfig::MAP_GROUND_Y - (GameConfig::PLAYER_JUMP_HEIGHT * t);
    }

    if (jumpFrame >= half) {
        const float t = (jumpFrame - half) / half;
        y = (GameConfig::MAP_GROUND_Y - GameConfig::PLAYER_JUMP_HEIGHT) + (GameConfig::PLAYER_JUMP_HEIGHT * t);
    }

    ++jumpFrame;

    if (jumpFrame < GameConfig::PLAYER_JUMP_DURATION) {
        return;
    }

    // 착지
    y = GameConfig::MAP_GROUND_Y;
    jumping = false;
    jumpCooldown = GameConfig::PLAYER_JUMP_COOLDOWN_MAX;

    // 착지 시 액션 초기화 (공중 모션 지속 방지)
    if (action == PlayerAction::ATTACK || action == PlayerAction::DEFEND) {
        action = PlayerAction::IDLE;
        actionFrame = 0;
    }
}

void Player::update() {
    // ===== 액션 프레임 관리 =====
    if (action == PlayerAction::ATTACK || action == PlayerAction::DEFEND) {
        if (actionFrame > 0) {
            --actionFrame;
        }

        if (actionFrame == 0) {
            action = PlayerAction::IDLE;
        }
    }

    // ===== 점프 모션 처리 =====
    if (jumping) {
        applyJumpMotion();
    }

    // ===== 점프 쿨타임 =====
    if (jumpCooldown > 0) {
        --jumpCooldown;
    }

    // ===== 키 릴리즈 감지 =====
    const bool zPressed = (GetAsyncKeyState('Z') & 0x8000) != 0;
    const bool downPressed = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;
    const bool upPressed = (GetAsyncKeyState(VK_UP) & 0x8000) != 0;

    if (!zPressed) {
        canAttack = true;
    }

    if (!downPressed) {
        canDefend = true;
    }

    if (!jumping && !upPressed) {
        canJump = true;
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

PlayerAction Player::getAction() const {
    return action;
}
