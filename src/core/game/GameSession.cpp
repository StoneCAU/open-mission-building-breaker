#include "GameSession.h"
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
    hitThisFrame = false;  // 프레임이 바뀔 때 초기화
    player.update();
    checkAndHandleCollision();
    buildingManager.updateAll();
}

void GameSession::checkGroundCollision() {
    auto& buildings = buildingManager.getAll();

    for (auto& building : buildings) {
        if (building.isDestroyed()) continue;

        // 빌딩이 지면에 닿았으면
        if (building.isOnGround()) {
            decreaseLife();
            resetCombo();
            messageQueue.push(MessageType::PLAYER_DAMAGED);
            // 빌딩은 제거하지 않음 (화면 밖으로 나가야 제거)
        }
    }
}

void GameSession::checkAndHandleCollision() {
    if (hitThisFrame) return;

    const float playerTopY = player.getY() - GameConfig::PLAYER_HEIGHT;
    Building* building = buildingManager.getBuildingAt(player.getX(), playerTopY);

    if (building == nullptr) return;

    // 점프해서 위로 올라가는 중일 때만 머리 충돌 체크
    if (player.getVelocityY() < 0) {
        CollisionResult result = player.processCollision(*building);
        if (result.type == CollisionResult::Type::ATTACK_HIT) {
            hitThisFrame = true;
            lastHitBuilding = building;
        }
        applyCollisionEffect(result);
        handleCollisionResult(result);
    }
}

void GameSession::applyCollisionEffect(const CollisionResult& result) {
    if (result.building == nullptr) return;

    if (result.type == CollisionResult::Type::ATTACK_HIT) {
        result.building->removeBottomFloor();  // takeHit() 대신
        return;
    }

    if (result.type == CollisionResult::Type::HEAD_COLLISION_RELEASED) {
        result.building->removeBottomFloor();  // takeHit() 대신
        return;
    }

     if (result.type == CollisionResult::Type::DEFENSE_SUCCESS) {
         result.building->applyRebound();
         return;
     }

     if (result.type == CollisionResult::Type::PLAYER_DAMAGED) {
         result.building->applyRebound();
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

int GameSession::getPlayTimeSeconds() const {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
    return static_cast<int>(duration.count());
}
