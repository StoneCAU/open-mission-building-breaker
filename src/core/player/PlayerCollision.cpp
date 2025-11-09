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
    CollisionResult result = tryHandleHeadCollision(building);
    if (result.type != CollisionResult::Type::NONE) return result;

    result = tryHandleAttackRange(building);
    if (result.type != CollisionResult::Type::NONE) return result;

    result = tryHandleDefenseRange(building);
    if (result.type != CollisionResult::Type::NONE) return result;

    return handleBodyCollision(building);
}

CollisionResult PlayerCollision::tryHandleHeadCollision(Building& building) {
    if (!movement.isJumping()) return {CollisionResult::Type::NONE, nullptr};

    const float headY = y - 1.0f;
    const float buildingBottom = building.getY();

    if (headY > buildingBottom) return {CollisionResult::Type::NONE, nullptr};

    movement.forceFall(buildingBottom + 1.0f);

    if (action.getType() == PlayerActionType::ATTACK) {
        building.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &building};
    }

    if (action.getType() == PlayerActionType::DEFEND) {
        building.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
    }

    if (isGroundLevel(buildingBottom + 1.0f)) {
        return processHeadDamage(building);
    }

    return {CollisionResult::Type::NONE, nullptr};
}

CollisionResult PlayerCollision::tryHandleAttackRange(Building& building) {
    if (action.getType() != PlayerActionType::ATTACK) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    const float headY = y - 1.0f;
    const float buildingBottom = building.getY();

    if (buildingBottom > headY) return {CollisionResult::Type::NONE, nullptr};
    if (buildingBottom < headY - GameConfig::PLAYER_ATTACK_RANGE) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    building.takeHit();
    return {CollisionResult::Type::ATTACK_HIT, &building};
}

CollisionResult PlayerCollision::tryHandleDefenseRange(Building& building) {
    if (action.getType() != PlayerActionType::DEFEND) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    const float headY = y - 1.0f;
    const float buildingBottom = building.getY();

    if (buildingBottom > headY) return {CollisionResult::Type::NONE, nullptr};
    if (buildingBottom < headY - GameConfig::PLAYER_DEFENSE_RANGE) {
        return {CollisionResult::Type::NONE, nullptr};
    }

    building.rebound();
    return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
}

CollisionResult PlayerCollision::handleBodyCollision(Building& building) {
    if (!isGroundLevel(y)) {
        return processAirCollision(building);
    } else {
        return processGroundCollision(building);
    }
}

CollisionResult PlayerCollision::processHeadDamage(Building& building) {
    if (!isInvincible()) {
        takeDamage();
        building.rebound();
        return {CollisionResult::Type::PLAYER_DAMAGED, &building};
    } else {
        building.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
    }
}

CollisionResult PlayerCollision::processAirCollision(Building& building) {
    if (isInvincible()) {
        building.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
    }

    if (action.getType() == PlayerActionType::DEFEND) {
        building.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
    }

    if (action.getType() == PlayerActionType::ATTACK) {
        building.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &building};
    }

    movement.forceFall(y);
    return {CollisionResult::Type::NONE, nullptr};
}

CollisionResult PlayerCollision::processGroundCollision(Building& building) {
    if (isInvincible()) {
        building.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
    }

    if (action.getType() == PlayerActionType::ATTACK) {
        building.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &building};
    }

    if (action.getType() == PlayerActionType::DEFEND) {
        building.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &building};
    }

    takeDamage();
    building.rebound();
    return {CollisionResult::Type::PLAYER_DAMAGED, &building};
}

bool PlayerCollision::isGroundLevel(float y) const {
    return y >= GameConfig::MAP_GROUND_Y - 0.1f;
}

void PlayerCollision::takeDamage() {
    if (damageFrame > 0) return;
    damaged = true;
    damageFrame = 30;
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
        if (damageFrame == 0) damaged = false;
    }
}
