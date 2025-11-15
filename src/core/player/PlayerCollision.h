#pragma once

class PlayerCollision {
public:
    PlayerCollision();

    void reset();
    void update();
    void takeDamage();

    bool isDamaged() const;

private:
    static constexpr int INVINCIBILITY_FRAMES = 30;

    int invincibilityFrame;
};
