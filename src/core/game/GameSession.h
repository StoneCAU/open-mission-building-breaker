#pragma once
#include "../../ui/InputHandler.h"
#include "../building/BuildingManager.h"
#include "../player/Player.h"

class GameSession {
public:
    GameSession();

    void start();
    void reset();
    void handleInput(InputKey key);
    void update();

    const Player& getPlayer() const;
    BuildingManager& getBuildingManager();
    const BuildingManager& getBuildingManager() const;

    int getScore() const;
    int getCombo() const;
    int getGauge() const;
    int getLife() const;
    bool isGameOver() const;

private:
    static constexpr int INITIAL_SCORE = 0;
    static constexpr int INITIAL_COMBO = 0;
    static constexpr int INITIAL_GAUGE = 0;
    static constexpr int INITIAL_LIFE = 3;

    Player player;
    BuildingManager buildingManager;

    int score;
    int combo;
    int gauge;
    int life;

    void checkAndHandleCollision();
    void handleCollisionResult(const CollisionResult& result);
    
    void onAttackHit();
    void onPlayerDamaged();
    
    void addScore(int value);
    void addCombo();
    void resetCombo();
    void decreaseLife();
};