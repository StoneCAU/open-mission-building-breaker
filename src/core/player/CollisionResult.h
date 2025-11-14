#pragma once

class Building;

struct CollisionResult {
    enum class Type {
        NONE,
        ATTACK_HIT,
        DEFENSE_SUCCESS,
        PLAYER_DAMAGED
    };
    
    Type type;
    Building* building;
};