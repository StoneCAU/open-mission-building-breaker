#pragma once
#include <string>

enum class FloorType {
    NORMAL,
    STRONG,
    WEAK
};

class Floor {
public:
    static constexpr int NORMAL_HP = 1;
    static constexpr int STRONG_HP = 2;
    static constexpr int WEAK_HP = 1;

    static constexpr char NORMAL_CHAR = '#';
    static constexpr char STRONG_CHAR = '=';
    static constexpr char WEAK_CHAR = '-';

    explicit Floor(FloorType type);

    void takeDamage(int damage);

    bool isDestroyed() const;
    int getHp() const;
    FloorType getType() const;
    std::string getVisual() const;

private:
    FloorType type;
    int hp;
    int maxHp;
    bool destroyed;
    std::string visual;
};
