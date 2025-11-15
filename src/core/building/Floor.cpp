#include "Floor.h"

#include "Building.h"

Floor::Floor(FloorType type)
    : type(type), destroyed(false) {

    if (type == FloorType::NORMAL) {
        maxHp = NORMAL_HP;
        visual = std::string(Building::WIDTH, NORMAL_CHAR);
        hp = maxHp;
        return;
    }

    if (type == FloorType::STRONG) {
        maxHp = STRONG_HP;
        visual = std::string(Building::WIDTH, STRONG_CHAR);
        hp = maxHp;
        return;
    }

    maxHp = WEAK_HP;
    visual = std::string(Building::WIDTH, WEAK_CHAR);
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
