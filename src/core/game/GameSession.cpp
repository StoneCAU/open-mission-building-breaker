#include "GameSession.h"
#include "GameConfig.h"
#include "../building/Building.h"

GameSession::GameSession()
    : score(GameConfig::INITIAL_SCORE),
      combo(GameConfig::INITIAL_COMBO),
      gauge(GameConfig::INITIAL_GAUGE),
      life(GameConfig::INITIAL_LIFE) {}

void GameSession::start() {
    reset();
    buildingManager.initBuildings();
}

void GameSession::reset() {
    score = GameConfig::INITIAL_SCORE;
    combo = GameConfig::INITIAL_COMBO;
    gauge = GameConfig::INITIAL_GAUGE;
    life = GameConfig::INITIAL_LIFE;
}

void GameSession::handleInput(InputKey key) {
    player.handleInput(key);
}

void GameSession::update() {
    player.update();
    checkAndHandleCollision();
    buildingManager.updateAll();
}

void GameSession::checkAndHandleCollision() {
    const float playerTopY = player.getY() - GameConfig::PLAYER_HEIGHT;

    Building* building = buildingManager.getBuildingAt(player.getX(), playerTopY);
    if (building == nullptr) return;

    CollisionResult result = player.processCollision(*building);
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
}

void GameSession::onDefenseSuccess() {
    // 추후 방어 성공 보너스 구현 시 사용
}

void GameSession::onPlayerDamaged() {
    decreaseLife();
    resetCombo();
}

void GameSession::addScore(int value) {
    score += value;
}

void GameSession::addCombo() {
    ++combo;
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
