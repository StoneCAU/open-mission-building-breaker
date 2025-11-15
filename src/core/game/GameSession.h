#pragma once

#include <chrono>

#include "GameOverDisplayData.h"
#include "../../ui/UIMessage.h"
#include "../player/Player.h"
#include "../building/BuildingManager.h"

class GameSession {
public:
    GameSession();

    void start();
    void reset();
    void handleInput(InputKey key);
    void update();

    bool isGameOver() const;

    const Player& getPlayer() const;
    BuildingManager& getBuildingManager();
    const BuildingManager& getBuildingManager() const;
    UIMessageQueue messageQueue;

    int getScore() const;
    int getCombo() const;
    int getMaxCombo() const;
    int getGauge() const;
    int getLife() const;
    int getPlayTimeSeconds() const;
    GameOverDisplayData getGameOverData(int currentHighScore) const;

private:
    Player player;
    BuildingManager buildingManager;

    int score;
    int combo;
    int maxCombo;
    int gauge;
    int life;
    std::chrono::steady_clock::time_point startTime;

    // 충돌 처리
    void checkAndHandleCollision();
    void checkPhysicsCollision();
    void checkActionCollision();
    void checkGroundCollision();

    // 게임 이벤트 처리
    void onAttackHit();
    void onDefenseSuccess();
    void onPlayerDamaged();

    // 점수/상태 관리
    void addScore(int value);
    void addCombo();
    void resetCombo();
    void decreaseLife();
    void addGauge(int value);
    void resetGauge();

    void executeUltimate();

    Building* lastHitBuilding = nullptr;
    bool hitThisFrame = false;
};
