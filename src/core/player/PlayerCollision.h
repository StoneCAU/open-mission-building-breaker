#pragma once
#include "CollisionResult.h"
#include "PlayerActionType.h"

class Building;
class PlayerMovement;
class PlayerAction;

class PlayerCollision {
public:
    PlayerCollision(int& x, float& y,
                   PlayerMovement& movement,
                   PlayerAction& action);

    CollisionResult processCollision(Building& building);

    void takeDamage();
    bool isInvincible() const;
    bool isDamaged() const;
    void update();

private:
    int& x;
    float& y;
    PlayerMovement& movement;
    PlayerAction& action;

    bool damaged;
    int damageFrame;

    // 충돌 처리 메인 로직
    CollisionResult tryHandleHeadCollision(Building& b);
    CollisionResult tryHandleActionRange(Building& b);
    CollisionResult handleBodyCollision(Building& b);

    // 액션별 처리
    CollisionResult handleActionByType(PlayerActionType type, Building& b);
    CollisionResult createAttackResult(Building& b);
    CollisionResult createDefenseResult(Building& b);
    CollisionResult createDamageResult(Building& b);

    // 헬퍼 메서드
    CollisionResult processHeadDamage(Building& b);
    bool isInActionRange(float playerTopY, float buildingBottomY, PlayerActionType type) const;

    // 유틸리티
    static bool isValidResult(const CollisionResult& result);
    bool isGroundLevel(float y) const;
    float getPlayerTopY() const;
    float getBuildingBottomY(const Building& b) const;
};
