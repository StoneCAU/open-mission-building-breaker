#include "Floor.h"
#include "../game/GameConfig.h"

Floor::Floor(FloorType type)
    : type(type), destroyed(false) {

    if (type == FloorType::NORMAL) {
        maxHp = GameConfig::FLOOR_NORMAL_HP;
        visual = std::string(GameConfig::BUILDING_WIDTH, GameConfig::FLOOR_NORMAL_CHAR);
    }

    if (type == FloorType::STRONG) {
        maxHp = GameConfig::FLOOR_STRONG_HP;
        visual = std::string(GameConfig::BUILDING_WIDTH, GameConfig::FLOOR_STRONG_CHAR);
    }

    if (type == FloorType::WEAK) {
        maxHp = GameConfig::FLOOR_WEAK_HP;
        visual = std::string(GameConfig::BUILDING_WIDTH, GameConfig::FLOOR_WEAK_CHAR);
    }

    hp = maxHp;
}

void Floor::takeDamage(int damage) {
    if (destroyed) return;

    hp -= damage;
    if (hp <= 0) {
        destroyed = true;
    }
}

bool Floor::isDestroyed() const {
    return destroyed;
}

int Floor::getHp() const {
    return hp;
}

FloorType Floor::getType() const {
    return type;
}

std::string Floor::getVisual() const {
    return visual;
}
