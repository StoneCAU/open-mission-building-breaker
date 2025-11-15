#pragma once

class GameStats {
public:
    GameStats();

    void reset();

    void addScore(int value);
    int getScore() const;

    void addCombo();
    void resetCombo();
    int getCombo() const;
    int getMaxCombo() const;

    void addGauge(int value);
    void resetGauge();
    int getGauge() const;
    bool canUseUltimate() const;

    void decreaseLife();
    int getLife() const;
    bool isAlive() const;

private:
    int score;
    int combo;
    int maxCombo;
    int gauge;
    int life;
};
