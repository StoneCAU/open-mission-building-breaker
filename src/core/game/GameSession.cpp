#include "GameSession.h"

#include <iostream>

#include "GameConfig.h"
#include "../building/Building.h"

GameSession::GameSession()
    : score(GameConfig::INITIAL_SCORE),
      combo(GameConfig::INITIAL_COMBO),
      gauge(GameConfig::INITIAL_GAUGE),
      life(GameConfig::INITIAL_LIFE),
      maxCombo(0){}

void GameSession::start() {
    startTime = std::chrono::steady_clock::now();
    reset();
}

void GameSession::reset() {
    score = GameConfig::INITIAL_SCORE;
    combo = GameConfig::INITIAL_COMBO;
    gauge = GameConfig::INITIAL_GAUGE;
    life = GameConfig::INITIAL_LIFE;
    maxCombo = 0;

    messageQueue.clear();
    player.reset();
    buildingManager.initBuildings();
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

    // 플레이어 업데이트 전 데미지 상태 기록
    bool wasDamaged = player.isDamaged();

    player.update();

    // 방금 데미지 입었으면 라이프 감소
    if (!wasDamaged && player.isDamaged()) {
        decreaseLife();
        resetCombo();
        messageQueue.push(MessageType::PLAYER_DAMAGED);
    }

    checkPhysicsCollision();
    checkActionCollision();
    buildingManager.updateAll();
}

void GameSession::checkPhysicsCollision() {
    if (player.isAttachedToBuilding()) {
        return;
    }

    const int px = player.getX();
    const float py = player.getY();
    const float playerTopY = py - GameConfig::PLAYER_HEIGHT;

    // 플레이어 위 3칸 범위에 빌딩 있는지 체크
    Building* building = buildingManager.getBuildingAbove(px, playerTopY, 1.0f);

    if (building == nullptr) {
        return;
    }

    float buildingBottom = building->getBottomY();

    // 위로 올라가는 중 + 충돌 예상
    if (player.getVelocityY() < -0.1f && playerTopY <= buildingBottom + 1.0f) {
        player.handlePhysicsCollision(buildingBottom);
        player.attachToBuilding(building);
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
            if (attackBuilding->getBottomY() > GameConfig::MAP_GROUND_Y) {
                return;
            }

            hitThisFrame = true;
            attackBuilding->removeBottomFloor();
            onAttackHit();

            if (player.isAttachedToBuilding()) {
                player.detachFromBuilding();
            }

            return;
        }
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

            // 방어하면 떼어냄 (이미 있음!)
            if (player.isAttachedToBuilding()) {
                player.detachFromBuilding();
            }

            return;
        }
    }

    if (!player.isDamaged()) {
        // 점프 중이거나 붙어있으면 데미지 X
        if (player.isJumping() || player.isAttachedToBuilding()) {
            return;
        }

        Building* building = buildingManager.getBuildingAbovePlayer(px, playerTopY, 0.5f);

        if (building && building->getVelocityY() > 0.01f) {
            player.takeDamage();
            building->applyRebound();
            onPlayerDamaged();
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
