#include "Player.h"
#include "GameConfig.h"
#include "../ui/InputHandler.h"
#include <windows.h>

namespace InputUtils {
    inline bool isPressed(int key) {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }

    inline bool isReleased(int key) {
        return !isPressed(key);
    }
}

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
      actionFrame(0),
      damaged(false),
      damageFrame(0) {}

/** ===================== 입력 처리 ===================== **/

void Player::handleInput(InputKey key) {
    if (jumping) {
        handleAirInput(key);
        return;
    }
    handleGroundInput(key);
}

void Player::handleGroundInput(InputKey key) {
    if (tryAction(key, canAttack, PlayerAction::ATTACK)) return;
    if (tryAction(key, canDefend, PlayerAction::DEFEND)) return;
    if (tryJump(key)) return;

    handleMovement(key);
    action = PlayerAction::IDLE;
}

void Player::handleAirInput(InputKey key) {
    if (tryAction(key, canAttack, PlayerAction::ATTACK)) return;
    if (tryAction(key, canDefend, PlayerAction::DEFEND)) return;
}

void Player::handleMovement(InputKey key) {
    if (key == InputKey::LEFT && canMoveLeft()) {
        --x;
    }
    if (key == InputKey::RIGHT && canMoveRight()) {
        ++x;
    }
}

/** ===================== 액션 처리 ===================== **/

bool Player::tryAction(InputKey key, bool& canDo, PlayerAction type) {
    if (type == PlayerAction::ATTACK && key != InputKey::ATTACK) return false;
    if (type == PlayerAction::DEFEND && key != InputKey::DEFEND) return false;

    // 방어는 지속형
    if (type == PlayerAction::DEFEND) {
        action = PlayerAction::DEFEND;
        actionFrame = 0;
        return true;
    }

    if (!canDo) return false;

    action = type;
    actionFrame = GameConfig::PLAYER_ACTION_DURATION;
    canDo = false;
    return true;
}

bool Player::tryJump(InputKey key) {
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

/** ===================== 점프 로직 ===================== **/

void Player::jump() {
    jumping = true;
    canJump = false;
    jumpFrame = 0;
}

void Player::applyJumpMotion() {
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

void Player::applyJumpRise(float half) {
    const float t = jumpFrame / half;
    y = GameConfig::MAP_GROUND_Y - (GameConfig::PLAYER_JUMP_HEIGHT * t);
    ++jumpFrame;
}

void Player::applyJumpFall(float half) {
    const float t = (jumpFrame - half) / half;
    y = (GameConfig::MAP_GROUND_Y - GameConfig::PLAYER_JUMP_HEIGHT)
      + (GameConfig::PLAYER_JUMP_HEIGHT * t);
    ++jumpFrame;
}

void Player::finishJump() {
    y = GameConfig::MAP_GROUND_Y;
    jumping = false;
    jumpCooldown = GameConfig::PLAYER_JUMP_COOLDOWN_MAX;

    if (action == PlayerAction::ATTACK || action == PlayerAction::DEFEND) {
        action = PlayerAction::IDLE;
        actionFrame = 0;
    }
}

/** ===================== 프레임 업데이트 ===================== **/

void Player::update() {
    updateActionFrame();

    if (jumping) {
        applyJumpMotion();
    }

    if (jumpCooldown > 0) {
        --jumpCooldown;
    }

    if (damageFrame > 0) {
        --damageFrame;
        if (damageFrame == 0) damaged = false;
    }

    updateKeyRelease();
}

void Player::updateActionFrame() {
    if (action == PlayerAction::DEFEND) return;

    if (actionFrame > 0) {
        --actionFrame;
    }

    if (actionFrame == 0 && action == PlayerAction::ATTACK) {
        action = PlayerAction::IDLE;
    }
}

/** ===================== 입력 해제 감지 ===================== **/

void Player::updateKeyRelease() {
    using namespace InputUtils;

    if (isReleased('Z')) canAttack = true;

    if (isReleased(VK_DOWN) && action == PlayerAction::DEFEND)
        action = PlayerAction::IDLE;

    if (!jumping && isReleased(VK_UP))
        canJump = true;
}

/** ===================== 유틸 ===================== **/

bool Player::canMoveLeft() const {
    return x > GameConfig::MAP_MIN_X;
}

bool Player::canMoveRight() const {
    return x < GameConfig::MAP_MAX_X;
}

/** ===================== 피격 처리 ===================== **/

void Player::takeDamage() {
    if (damageFrame > 0) return; // 무적 시간 중
    damaged = true;
    damageFrame = 20; // 약 20프레임 지속
}

/** ===================== Getter ===================== **/

bool Player::isJumping() const {
    return jumping;
}

int Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

PlayerAction Player::getAction() const {
    return action;
}

bool Player::isDamaged() const {
    return damaged;
}
