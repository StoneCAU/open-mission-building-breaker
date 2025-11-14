#include "Player.h"
#include "../building/Building.h"
#include "../game/GameConfig.h"

Player::Player()
    : x(GameConfig::PLAYER_START_X),
      y(static_cast<float>(GameConfig::MAP_GROUND_Y)),
      movement(x, y),
      collision(x, y, movement, action),
      stuckUnderBuilding(false),
      stuckBuilding(nullptr) {}

void Player::handleInput(InputKey key) {
    if (collision.isDamaged()) return;

    action.handleInput(key);

    if (movement.handleInput(key)) {
        action.setActionCooldown(GameConfig::PLAYER_JUMP_COOLDOWN_MAX);
    }
}

void Player::update() {
    // 1. 빌딩에 찧혀있으면 빌딩 따라 이동
    updateStuckState();

    // 2. 일반 업데이트
    action.update();
    movement.update();
    collision.update();
}

void Player::updateStuckState() {
    if (!stuckUnderBuilding || stuckBuilding == nullptr) {
        return;
    }

    // 빌딩이 파괴되었거나 해제 조건이면 놓아줌
    if (stuckBuilding->isDestroyed() || shouldReleaseFromBuilding()) {
        releaseFromBuilding();
        return;
    }

    // 빌딩 하단에 계속 붙어있기
    const float newY = stuckBuilding->getY() + GameConfig::PLAYER_HEIGHT;
    movement.setY(newY);
    movement.stopVerticalMovement();
}

bool Player::shouldReleaseFromBuilding() const {
    // 지상까지 눌렸으면 해제
    const float currentY = stuckBuilding->getY() + GameConfig::PLAYER_HEIGHT;
    return currentY >= GameConfig::MAP_GROUND_Y - 0.1f;
}

CollisionResult Player::processCollision(Building& building) {
    CollisionResult result = collision.processCollision(building);

    // 충돌 결과에 따라 상태 변경
    handleCollisionResult(result);

    return result;
}

void Player::handleCollisionResult(const CollisionResult& result) {
    if (result.type == CollisionResult::Type::HEAD_COLLISION_STUCK) {
        if (!stuckUnderBuilding) {
            stickToBuilding(result.building);
        }
        return;
    }

    if (result.type == CollisionResult::Type::HEAD_COLLISION_RELEASED) {
        releaseFromBuilding();
        return;
    }

    if (result.type == CollisionResult::Type::PLAYER_DAMAGED) {
        releaseFromBuilding();
        return;
    }

    // 방어 성공 시 즉시 찧힌 상태 해제 (플레이어는 그대로 떨어짐)
    if (result.type == CollisionResult::Type::DEFENSE_SUCCESS) {
        releaseFromBuilding();
    }
}

void Player::stickToBuilding(Building* building) {
    stuckUnderBuilding = true;
    stuckBuilding = building;
}

void Player::releaseFromBuilding() {
    stuckUnderBuilding = false;
    stuckBuilding = nullptr;
}

bool Player::isStuckUnderBuilding() const {
    return stuckUnderBuilding;
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

float Player::getVelocityY() const {
    return movement.getVelocityY();
}

PlayerActionType Player::getAction() const {
    return action.getType();
}

bool Player::isDamaged() const {
    return collision.isDamaged();
}
