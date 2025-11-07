#pragma once
#include "BuildingManager.h"
#include "Player.h"

class GameSession {
public:
    GameSession();

    void start();
    void update();
    void handleInput(InputKey key);

    const Player& getPlayer() const;

    BuildingManager& getBuildingManager();
    const BuildingManager& getBuildingManager() const;

    void addScore(int value);
    void addCombo();
    void resetCombo();
    void decreaseLife();

    bool isGameOver() const;

    int getScore() const;
    int getCombo() const;
    int getGauge() const;
    int getLife() const;

private:
    void reset();
    void checkCollisions();

    Player player;
    BuildingManager buildingManager;

    int score;
    int combo;
    int gauge;
    int life;

    static constexpr int INITIAL_LIFE = 3;
    static constexpr int INITIAL_GAUGE = 0;
    static constexpr int INITIAL_SCORE = 0;
    static constexpr int INITIAL_COMBO = 0;
};
