#include "Player.h"
#include "Building.h"
#include <windows.h>

#include "GameConfig.h"

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
      actionCooldown(0) {}

/** ===================== 입력 처리 ===================== **/

void Player::handleInput(InputKey key) {
    if (damaged) return;

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

bool Player::tryAction(InputKey key, bool& canDo, PlayerAction type) {
    if (type == PlayerAction::ATTACK && key != InputKey::ATTACK) return false;
    if (type == PlayerAction::DEFEND && key != InputKey::DEFEND) return false;

    if (actionCooldown > 0) return false;

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
    actionCooldown = 15;
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

    if (action == PlayerAction::ATTACK) {
        action = PlayerAction::IDLE;
        actionFrame = 0;
    }
}

void Player::forceFall(float newY) {
    y = newY;

    if (jumping) {
        jumpFrame = GameConfig::PLAYER_JUMP_DURATION / 2.0f;
    }

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

void Player::updateKeyRelease() {
    using namespace InputUtils;

    if (isReleased('Z')) canAttack = true;

    if (isReleased(VK_DOWN) && action == PlayerAction::DEFEND)
        action = PlayerAction::IDLE;

    if (!jumping && isReleased(VK_UP))
        canJump = true;
}

/** ===================== 🔥 충돌 처리 ===================== **/

CollisionResult Player::processCollision(Building& b) {
    CollisionResult result = tryHandleHeadCollision(b);
    if (result.type != CollisionResult::Type::NONE) return result;

    result = tryHandleAttackRange(b);
    if (result.type != CollisionResult::Type::NONE) return result;

    result = tryHandleDefenseRange(b);
    if (result.type != CollisionResult::Type::NONE) return result;

    return handleBodyCollision(b);
}

CollisionResult Player::tryHandleHeadCollision(Building& b) {
    if (!jumping) return {CollisionResult::Type::NONE, nullptr};

    const float headY = y - 1.0f;
    const float buildingBottom = b.getY();

    if (headY > buildingBottom) return {CollisionResult::Type::NONE, nullptr};

    forceFall(buildingBottom + 1.0f);

    if (action == PlayerAction::ATTACK) {
        b.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &b};
    }

    if (action == PlayerAction::DEFEND) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (isGroundLevel(buildingBottom + 1.0f)) {
        return processHeadDamage(b);
    }

    return {CollisionResult::Type::NONE, nullptr};
}

CollisionResult Player::tryHandleAttackRange(Building& b) {
    if (action != PlayerAction::ATTACK) return {CollisionResult::Type::NONE, nullptr};

    const float headY = y - 1.0f;
    const float buildingBottom = b.getY();

    if (buildingBottom > headY) return {CollisionResult::Type::NONE, nullptr};
    if (buildingBottom < headY - GameConfig::PLAYER_ATTACK_RANGE) return {CollisionResult::Type::NONE, nullptr};

    b.takeHit();
    return {CollisionResult::Type::ATTACK_HIT, &b};
}

CollisionResult Player::tryHandleDefenseRange(Building& b) {
    if (action != PlayerAction::DEFEND) return {CollisionResult::Type::NONE, nullptr};

    const float headY = y - 1.0f;
    const float buildingBottom = b.getY();

    if (buildingBottom > headY) return {CollisionResult::Type::NONE, nullptr};
    if (buildingBottom < headY - GameConfig::PLAYER_DEFENSE_RANGE) return {CollisionResult::Type::NONE, nullptr};

    b.rebound();
    return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
}

CollisionResult Player::handleBodyCollision(Building& b) {
    if (!isGroundLevel(y)) {
        return processAirCollision(b);
    } else {
        return processGroundCollision(b);
    }
}

CollisionResult Player::processHeadDamage(Building& b) {
    if (!isInvincible()) {
        takeDamage();
        b.rebound();
        return {CollisionResult::Type::PLAYER_DAMAGED, &b};
    } else {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }
}

CollisionResult Player::processAirCollision(Building& b) {
    if (isInvincible()) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (action == PlayerAction::DEFEND) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (action == PlayerAction::ATTACK) {
        b.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &b};
    }

    forceFall(y);
    return {CollisionResult::Type::NONE, nullptr};
}

CollisionResult Player::processGroundCollision(Building& b) {
    if (isInvincible()) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (action == PlayerAction::ATTACK) {
        b.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &b};
    }

    if (action == PlayerAction::DEFEND) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    takeDamage();
    b.rebound();
    return {CollisionResult::Type::PLAYER_DAMAGED, &b};
}

/** ===================== 유틸 ===================== **/

bool Player::isGroundLevel(float y) const {
    return y >= GameConfig::MAP_GROUND_Y - 0.1f;
}

bool Player::canMoveLeft() const {
    return x > GameConfig::MAP_MIN_X;
}

bool Player::canMoveRight() const {
    return x < GameConfig::MAP_MAX_X;
}

void Player::takeDamage() {
    if (damageFrame > 0) return;
    damaged = true;
    damageFrame = 30;
}

bool Player::isInvincible() const {
    return damageFrame > 0;
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
