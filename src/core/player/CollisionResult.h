#pragma once

class Building;

struct CollisionResult {
    enum class Type {
        NONE,
        ATTACK_HIT,
        DEFENSE_SUCCESS,
        PLAYER_DAMAGED,
        HEAD_COLLISION_STUCK,
        HEAD_COLLISION_RELEASED
    };
    
    Type type;
    Building* building;
};