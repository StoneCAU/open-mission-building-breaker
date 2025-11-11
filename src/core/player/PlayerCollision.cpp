#include "PlayerCollision.h"
#include "../building/Building.h"
#include "PlayerMovement.h"
#include "PlayerAction.h"
#include "../game/GameConfig.h"

PlayerCollision::PlayerCollision(int& x, float& y,
                                 PlayerMovement& movement,
                                 PlayerAction& action)
    : x(x), y(y),
      movement(movement),
      action(action),
      damaged(false),
      damageFrame(0) {}

CollisionResult PlayerCollision::processCollision(Building& building) {
    CollisionResult result;

    result = tryHandleHeadCollision(building);
    if (isValidResult(result)) return result;

    result = tryHandleActionRange(building);
    if (isValidResult(result)) return result;

    return handleBodyCollision(building);
}

bool PlayerCollision::isValidResult(const CollisionResult& result) {
    return result.type != CollisionResult::Type::NONE;
}

float PlayerCollision::getPlayerTopY() const {
    return y - GameConfig::PLAYER_HEIGHT;
}

float PlayerCollision::getBuildingBottomY(const Building& b) const {
    return b.getY();
}

CollisionResult PlayerCollision::handleActionByType(PlayerActionType type, Building& building) {
    if (type == PlayerActionType::ATTACK) {
        return createAttackResult(building);
    }

    if (type == PlayerActionType::DEFEND) {
        return createDefenseResult(building);
    }

    return {CollisionResult::Type::NONE, nullptr};
}

bool PlayerCollision::isInActionRange(float playerTopY, float buildingBottomY, PlayerActionType type) const {
    if (buildingBottomY > playerTopY) return false;

    if (type == PlayerActionType::ATTACK) {
        return buildingBottomY >= playerTopY - GameConfig::PLAYER_ATTACK_RANGE;
    }

    if (type == PlayerActionType::DEFEND) {
        return buildingBottomY >= playerTopY - GameConfig::PLAYER_DEFENSE_RANGE;
    }

    return false;
}

CollisionResult PlayerCollision::tryHandleHeadCollision(Building& building) {
    if (!movement.isJumping()) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    const float playerTopY = getPlayerTopY();
    const float buildingBottomY = getBuildingBottomY(building);

    if (playerTopY > buildingBottomY) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    const PlayerActionType actionType = action.getType();

    // ===== 공격 중이면 forceFall 금지 =====
    if (actionType == PlayerActionType::ATTACK) {
        if (!action.isAttackFirstFrame()) {
            return {CollisionResult::Type::NONE, nullptr};
        }
        // 건물 부순 후에는 위치 보정하지 않음
        return {CollisionResult::Type::HEAD_COLLISION_RELEASED, &building};
    }

    // ===== 공격 아닐 때만 위치 보정 =====
    movement.forceFall(buildingBottomY + GameConfig::PLAYER_HEIGHT);

    if (actionType == PlayerActionType::DEFEND) {
        return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
    }

    if (isGroundLevel(buildingBottomY + GameConfig::PLAYER_HEIGHT)) {
        takeDamage();
        return {CollisionResult::Type::PLAYER_DAMAGED, &building};
    }

    return {CollisionResult::Type::HEAD_COLLISION_STUCK, &building};
}

CollisionResult PlayerCollision::tryHandleActionRange(Building& building) {
    const PlayerActionType actionType = action.getType();

    if (actionType == PlayerActionType::IDLE) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    const float playerTopY = getPlayerTopY();
    const float buildingBottomY = getBuildingBottomY(building);

    if (!isInActionRange(playerTopY, buildingBottomY, actionType)) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    return handleActionByType(actionType, building);
}

CollisionResult PlayerCollision::handleBodyCollision(Building& building) {
    if (isInvincible()) {
        return createDefenseResult(building);
    }

    const PlayerActionType actionType = action.getType();

    if (actionType != PlayerActionType::IDLE) {
        return handleActionByType(actionType, building);
    }

    return createDamageResult(building);
}

CollisionResult PlayerCollision::processHeadDamage(Building& building) {
    if (isInvincible()) {
        return createDefenseResult(building);
    }

    takeDamage();
    return {CollisionResult::Type::PLAYER_DAMAGED, &building};
}

CollisionResult PlayerCollision::createAttackResult(Building& building) {
    // Building 조작 제거 - GameSession에서 처리
    return {CollisionResult::Type::ATTACK_HIT, &building};
}

CollisionResult PlayerCollision::createDefenseResult(Building& building) {
    // Building 조작 제거 - GameSession에서 처리
    return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
}

CollisionResult PlayerCollision::createDamageResult(Building& building) {
    takeDamage();
    // Building 조작 제거 - GameSession에서 처리
    return {CollisionResult::Type::PLAYER_DAMAGED, &building};
}

bool PlayerCollision::isGroundLevel(float y) const {
    return y >= GameConfig::MAP_GROUND_Y - 0.1f;
}

void PlayerCollision::takeDamage() {
    if (damageFrame > 0) return;
    damaged = true;
    damageFrame = GameConfig::PLAYER_INVINCIBILITY_FRAMES;
}

bool PlayerCollision::isInvincible() const {
    return damageFrame > 0;
}

bool PlayerCollision::isDamaged() const {
    return damaged;
}

void PlayerCollision::update() {
    if (damageFrame > 0) {
        --damageFrame;
        if (damageFrame == 0) {
            damaged = false;
        }
    }
}
