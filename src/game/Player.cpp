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
      damageFrame(0),
      hitHead(false),
      actionCooldown(0){}

/** ===================== 입력 처리 ===================== **/

void Player::handleInput(InputKey key) {
    if (damaged) {
        return;
    }

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

    // 🔥 점프 직후 쿨타임 체크
    if (actionCooldown > 0) return false;

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
    actionCooldown = 15;  // 🔥 점프 후 3프레임 동안 액션 불가 (약 0.06초)
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

    // 🔥 방어는 유지, 공격만 초기화
    if (action == PlayerAction::ATTACK) {
        action = PlayerAction::IDLE;
        actionFrame = 0;
    }
}

void Player::forceFall(float newY) {
    y = newY;

    if (jumping) {
        // 점프 중이면 낙하 구간으로 전환
        jumpFrame = GameConfig::PLAYER_JUMP_DURATION / 2.0f;
    }

    // 🔥 땅에 닿았으면 점프 종료 처리
    if (y >= GameConfig::MAP_GROUND_Y - 0.1f) {
        finishJump();
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

    // 🔥 액션 쿨타임 감소
    if (actionCooldown > 0) {
        --actionCooldown;
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
    if (damageFrame > 0) return;
    damaged = true;
    damageFrame = 30;
}

bool Player::isInvincible() const {
    return damageFrame > 0;
}

void Player::onReboundCollision(const Building& b, bool isGround) {
    if (isGround) {
        // 지상에서 건물 반동 — 튕겨나감
        y = GameConfig::MAP_GROUND_Y;
        return;
    }

    // 공중 충돌 — 강제로 낙하 시작
    forceFall(b.getY() + 1.0f);
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
