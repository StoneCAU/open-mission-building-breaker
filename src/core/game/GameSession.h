#pragma once

#include <chrono>

#include "GameOverDisplayData.h"
#include "GameStats.h"
#include "../../platform/console/UIMessage.h"
#include "../player/Player.h"
#include "../building/BuildingManager.h"
#include "../../interfaces/InputKey.h"
#include "../../interfaces/IInputHandler.h"

class GameSession {
public:
    GameSession();

    void start();
    void reset();
    void handleInput(InputKey key);
    void update(IInputHandler* inputHandler);
    void clearUltimateFlag() { ultimateUsedThisFrame = false; }

    bool isGameOver() const;
    bool justUsedUltimate() const { return ultimateUsedThisFrame; }

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
    GameStats stats;
    std::chrono::steady_clock::time_point startTime;

    bool hitThisFrame = false;
    bool ultimateUsedThisFrame = false;

    void updatePlayerState(IInputHandler* inputHandler);
    void updateCollisions();

    void checkPhysicsCollision();
    void checkAttackCollision();
    void checkDefendCollision();
    void checkDamageCollision();

    struct PlayerPosition {
        int x;
        float y;
        float topY;
    };
    PlayerPosition getPlayerPosition() const;

    Building* findBuildingForPhysics(const PlayerPosition& pos);
    Building* findBuildingForAttack(const PlayerPosition& pos);
    Building* findBuildingForDefend(const PlayerPosition& pos);
    Building* findBuildingForDamage(const PlayerPosition& pos);

    void executePhysicsAttachment(Building* building);
    void executeAttack(Building* building);
    void executeDefend(Building* building);
    void executeDamage(Building* building);

    bool canAttachToBuilding(Building* building, float playerTopY) const;
    bool canAttackBuilding(Building* building) const;
    bool canDamagePlayer(Building* building) const;
    void detachPlayerIfAttached();

    void executeUltimate();
    void handlePlayerDamaged();
    void handleAttackHit();
    void handleDefenseSuccess();
};
