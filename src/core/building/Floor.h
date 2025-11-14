#pragma once
#include <string>

enum class FloorType {
    NORMAL,
    STRONG,
    WEAK
};

class Floor {
public:
    Floor(FloorType type = FloorType::NORMAL);

    void takeDamage(int damage = 1);
    bool isDestroyed() const;

    int getHp() const;
    FloorType getType() const;
    std::string getVisual() const;

private:
    int hp;
    int maxHp;
    FloorType type;
    std::string visual;
    bool destroyed;
};
