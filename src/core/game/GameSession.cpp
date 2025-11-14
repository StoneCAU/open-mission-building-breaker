#include "GameSession.h"

#include <iostream>

#include "GameConfig.h"
#include "../building/Building.h"

GameSession::GameSession()
    : score(GameConfig::INITIAL_SCORE),
      combo(GameConfig::INITIAL_COMBO),
      gauge(GameConfig::INITIAL_GAUGE),
      life(GameConfig::INITIAL_LIFE),
      maxCombo(0),
      startTime(std::chrono::steady_clock::now()) {}

void GameSession::start() {
    reset();
    buildingManager.initBuildings();
}

void GameSession::reset() {
    score = GameConfig::INITIAL_SCORE;
    combo = GameConfig::INITIAL_COMBO;
    gauge = GameConfig::INITIAL_GAUGE;
    life = GameConfig::INITIAL_LIFE;
    maxCombo = 0;
    startTime = std::chrono::steady_clock::now();
}

void GameSession::handleInput(InputKey key) {
    if (key == InputKey::ULTIMATE && gauge >= GameConfig::ULTIMATE_GAUGE_COST) {
        executeUltimate();
        return;
    }

    player.handleInput(key);
}

void GameSession::executeUltimate() {
    int destroyedCount = buildingManager.getActiveCount();
    buildingManager.destroyAll();

    resetGauge();
    addScore(destroyedCount * GameConfig::SCORE_PER_ATTACK_HIT);

    messageQueue.push(MessageType::ULTIMATE_ACTIVATED, destroyedCount);
}

void GameSession::update() {
    hitThisFrame = false;

    player.update();
    buildingManager.updateAll();

    checkPhysicsCollision();
    checkActionCollision();
}

void GameSession::checkPhysicsCollision() {
    const int px = player.getX();
    const float py = player.getY();
    const float playerTopY = py - GameConfig::PLAYER_HEIGHT;

    Building* building = buildingManager.getBuildingAt(px, playerTopY);
    if (building == nullptr) return;

    // 플레이어가 위로 올라가는 중이고, 빌딩 밑면과 충돌
    if (player.getVelocityY() < 0 && playerTopY >= building->getBottomY() - 1.0f) {
        player.handlePhysicsCollision(building->getBottomY());
    }
}

void GameSession::checkActionCollision() {
    if (hitThisFrame) return;

    const PlayerActionType actionType = player.getAction();
    const int px = player.getX();
    const float py = player.getY();
    const float playerTopY = py - GameConfig::PLAYER_HEIGHT;

    // 공격 체크
    if (actionType == PlayerActionType::ATTACK && player.isAttackActiveFrame()) {
        Building* attackBuilding = buildingManager.getBuildingInRange(
            px,
            playerTopY,
            GameConfig::PLAYER_ATTACK_RANGE
        );

        if (attackBuilding) {
            hitThisFrame = true;
            attackBuilding->removeBottomFloor();
            onAttackHit();
        }
        return;
    }

    // 방어 체크
    if (actionType == PlayerActionType::DEFEND) {
        Building* defendBuilding = buildingManager.getBuildingInRange(
            px,
            playerTopY,
            GameConfig::PLAYER_DEFENSE_RANGE
        );

        if (defendBuilding) {
            defendBuilding->applyRebound();
            onDefenseSuccess();
        }
        return;
    }

    if (actionType == PlayerActionType::IDLE) {
        Building* building = buildingManager.getBuildingAbovePlayer(px, playerTopY, 0.5f);

        if (building && !player.isDamaged()) {
            if (building->getVelocityY() > 0.01f) {
                player.takeDamage();
                building->applyRebound();
                onPlayerDamaged();
            }
        }
    }
}
void GameSession::checkGroundCollision() {
    // 빌딩이 지면 닿으면 조용히 제거 (BuildingManager에서 처리 중)
    // 추가 로직 필요 시 여기 작성
}

void GameSession::onAttackHit() {
    addScore(GameConfig::SCORE_PER_ATTACK_HIT);
    addCombo();
    addGauge(10);
    messageQueue.push(MessageType::ATTACK_HIT, 1);
}

void GameSession::onDefenseSuccess() {
    messageQueue.push(MessageType::DEFENSE_SUCCESS);
}

void GameSession::onPlayerDamaged() {
    decreaseLife();
    resetCombo();
    messageQueue.push(MessageType::PLAYER_DAMAGED);
}

void GameSession::addScore(int value) {
    score += value;
}

void GameSession::addCombo() {
    ++combo;
    if (combo > maxCombo) {
        maxCombo = combo;
    }
}

void GameSession::resetCombo() {
    combo = 0;
}

void GameSession::decreaseLife() {
    if (life > 0) {
        --life;
    }
}

bool GameSession::isGameOver() const {
    return life <= 0;
}

const Player& GameSession::getPlayer() const {
    return player;
}

void GameSession::addGauge(int value) {
    gauge = std::min(100, gauge + value);
}

void GameSession::resetGauge() {
    gauge = 0;
}

BuildingManager& GameSession::getBuildingManager() {
    return buildingManager;
}

const BuildingManager& GameSession::getBuildingManager() const {
    return buildingManager;
}

int GameSession::getScore() const {
    return score;
}

int GameSession::getCombo() const {
    return combo;
}

int GameSession::getGauge() const {
    return gauge;
}

int GameSession::getLife() const {
    return life;
}

int GameSession::getMaxCombo() const {
    return maxCombo;
}

int GameSession::getPlayTimeSeconds() const {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
    return static_cast<int>(duration.count());
}
