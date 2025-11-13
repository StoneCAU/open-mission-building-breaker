#include "GameSession.h"
#include "GameConfig.h"
#include "../building/Building.h"

GameSession::GameSession()
    : score(GameConfig::INITIAL_SCORE),
      combo(GameConfig::INITIAL_COMBO),
      gauge(GameConfig::INITIAL_GAUGE),
      life(GameConfig::INITIAL_LIFE),
      maxCombo(0) {}

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
}

void GameSession::handleInput(InputKey key) {
    player.handleInput(key);
}

void GameSession::update() {
    hitThisFrame = false;  // 프레임이 바뀔 때 초기화
    player.update();
    checkAndHandleCollision();
    buildingManager.updateAll();
}

void GameSession::checkAndHandleCollision() {
    if (hitThisFrame) return; // 이미 이번 틱에서 처리했으면 무시

    const float playerTopY = player.getY() - GameConfig::PLAYER_HEIGHT;
    Building* building = buildingManager.getBuildingAt(player.getX(), playerTopY);
    if (building == nullptr) return;

    CollisionResult result = player.processCollision(*building);
    if (result.type == CollisionResult::Type::ATTACK_HIT) {
        hitThisFrame = true;         // 이 프레임에서 한 번만 허용
        lastHitBuilding = building;  // 기록
    }
    applyCollisionEffect(result);
    handleCollisionResult(result);
}

void GameSession::applyCollisionEffect(const CollisionResult& result) {
    if (result.building == nullptr) return;

    if (result.type == CollisionResult::Type::ATTACK_HIT) {
        result.building->takeHit();
        return;
    }

    if (result.type == CollisionResult::Type::HEAD_COLLISION_RELEASED) {
        result.building->takeHit();
        return;
    }

    if (result.type == CollisionResult::Type::DEFENSE_SUCCESS) {
        result.building->rebound();
        return;
    }

    if (result.type == CollisionResult::Type::PLAYER_DAMAGED) {
        result.building->rebound();
    }
}

void GameSession::handleCollisionResult(const CollisionResult& result) {
    if (result.type == CollisionResult::Type::ATTACK_HIT) {
        onAttackHit();
        return;
    }

    if (result.type == CollisionResult::Type::HEAD_COLLISION_RELEASED) {
        onAttackHit();
        return;
    }

    if (result.type == CollisionResult::Type::DEFENSE_SUCCESS) {
        onDefenseSuccess();
        return;
    }

    if (result.type == CollisionResult::Type::PLAYER_DAMAGED) {
        onPlayerDamaged();
    }
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
