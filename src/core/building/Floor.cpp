#include "Floor.h"
#include "Building.h"

Floor::Floor(FloorType type)
    : type(type),
      destroyed(false),
      destructionFrame(0) {

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
        startDestruction();
    }
}

void Floor::update() {
    if (destructionFrame > 0) {
        --destructionFrame;
    }
}

void Floor::startDestruction() {
    destroyed = true;
    destructionFrame = DESTRUCTION_EFFECT_FRAMES;
    visual = std::string(Building::WIDTH, DESTRUCTION_CHAR);
}

bool Floor::isDestroyed() const {
    return destroyed;
}

bool Floor::shouldRemove() const {
    return destroyed && destructionFrame == 0;
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
