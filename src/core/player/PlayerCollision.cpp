#include "PlayerCollision.h"
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

CollisionResult PlayerCollision::processCollision(Building& b) {
    CollisionResult result = tryHandleHeadCollision(b);
    if (result.type != CollisionResult::Type::NONE) return result;

    result = tryHandleAttackRange(b);
    if (result.type != CollisionResult::Type::NONE) return result;

    result = tryHandleDefenseRange(b);
    if (result.type != CollisionResult::Type::NONE) return result;

    return handleBodyCollision(b);
}

CollisionResult PlayerCollision::tryHandleHeadCollision(Building& b) {
    if (!movement.isJumping()) return {CollisionResult::Type::NONE, nullptr};

    const float headY = y - 1.0f;
    const float buildingBottom = b.getY();

    if (headY > buildingBottom) return {CollisionResult::Type::NONE, nullptr};

    movement.forceFall(buildingBottom + 1.0f);

    if (action.getType() == PlayerActionType::ATTACK) {
        b.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &b};
    }

    if (action.getType() == PlayerActionType::DEFEND) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (isGroundLevel(buildingBottom + 1.0f)) {
        return processHeadDamage(b);
    }

    return {CollisionResult::Type::NONE, nullptr};
}

CollisionResult PlayerCollision::tryHandleAttackRange(Building& b) {
    if (action.getType() != PlayerActionType::ATTACK) return {CollisionResult::Type::NONE, nullptr};

    const float headY = y - 1.0f;
    const float buildingBottom = b.getY();

    if (buildingBottom > headY) return {CollisionResult::Type::NONE, nullptr};
    if (buildingBottom < headY - GameConfig::PLAYER_ATTACK_RANGE) return {CollisionResult::Type::NONE, nullptr};

    b.takeHit();
    return {CollisionResult::Type::ATTACK_HIT, &b};
}

CollisionResult PlayerCollision::tryHandleDefenseRange(Building& b) {
    if (action.getType() != PlayerActionType::DEFEND) return {CollisionResult::Type::NONE, nullptr};

    const float headY = y - 1.0f;
    const float buildingBottom = b.getY();

    if (buildingBottom > headY) return {CollisionResult::Type::NONE, nullptr};
    if (buildingBottom < headY - GameConfig::PLAYER_DEFENSE_RANGE) return {CollisionResult::Type::NONE, nullptr};

    b.rebound();
    return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
}

CollisionResult PlayerCollision::handleBodyCollision(Building& b) {
    if (!isGroundLevel(y)) {
        return processAirCollision(b);
    } else {
        return processGroundCollision(b);
    }
}

CollisionResult PlayerCollision::processHeadDamage(Building& b) {
    if (!isInvincible()) {
        takeDamage();
        b.rebound();
        return {CollisionResult::Type::PLAYER_DAMAGED, &b};
    } else {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }
}

CollisionResult PlayerCollision::processAirCollision(Building& b) {
    if (isInvincible()) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (action.getType() == PlayerActionType::DEFEND) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (action.getType() == PlayerActionType::ATTACK) {
        b.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &b};
    }

    movement.forceFall(y);
    return {CollisionResult::Type::NONE, nullptr};
}

CollisionResult PlayerCollision::processGroundCollision(Building& b) {
    if (isInvincible()) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    if (action.getType() == PlayerActionType::ATTACK) {
        b.takeHit();
        return {CollisionResult::Type::ATTACK_HIT, &b};
    }

    if (action.getType() == PlayerActionType::DEFEND) {
        b.rebound();
        return {CollisionResult::Type::DEFENSE_SUCCESS, &b};
    }

    takeDamage();
    b.rebound();
    return {CollisionResult::Type::PLAYER_DAMAGED, &b};
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
