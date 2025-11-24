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

    int getComboScore(int baseScore) const;

private:
    int score;
    int combo;
    int gauge;
    int life;
    int maxCombo;

    static constexpr int COMBO_TIER_LOW = 5;
    static constexpr int COMBO_TIER_MID = 15;
    static constexpr float COMBO_MULTIPLIER_BASE = 1.0f;
    static constexpr float COMBO_MULTIPLIER_LOW = 1.5f;
    static constexpr float COMBO_MULTIPLIER_MID = 2.5f;
    static constexpr float COMBO_MULTIPLIER_HIGH = 4.0f;

    float calculateComboMultiplier() const;
};
