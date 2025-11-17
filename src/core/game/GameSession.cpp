#include "GameSession.h"

#include "GameConfig.h"
#include "../building/Building.h"
#include "../ui/UIMessage.h"

namespace {
    constexpr float PHYSICS_COLLISION_RANGE = 1.0f;
    constexpr float PHYSICS_COLLISION_THRESHOLD = 1.0f;
    constexpr float UPWARD_VELOCITY_THRESHOLD = -0.1f;
    constexpr float DAMAGE_COLLISION_RANGE = 0.5f;
    constexpr float FALLING_VELOCITY_THRESHOLD = 0.01f;
    constexpr int ATTACK_GAUGE_REWARD = 10;
}

GameSession::GameSession() = default;

void GameSession::start() {
    startTime = std::chrono::steady_clock::now();
    reset();
}

void GameSession::reset() {
    stats.reset();
    messageQueue.clear();
    player.reset();
    buildingManager.initBuildings();
    ultimateUsedThisFrame = false;
}

void GameSession::handleInput(InputKey key) {
    if (key == InputKey::ULTIMATE && stats.canUseUltimate()) {
        executeUltimate();
        return;
    }

    player.handleInput(key);
}

void GameSession::executeUltimate() {
    int destroyedCount = buildingManager.getActiveCount();
    buildingManager.damageAllFloors();

    stats.resetGauge();
    stats.addScore(destroyedCount * GameConfig::SCORE_PER_ATTACK_HIT);

    messageQueue.push(MessageType::ULTIMATE_ACTIVATED, destroyedCount);
    ultimateUsedThisFrame = true;
}

void GameSession::update(IInputHandler* inputHandler) {
    hitThisFrame = false;

    updatePlayerState(inputHandler);
    updateCollisions();
    buildingManager.updateAll();
    messageQueue.update();
}

void GameSession::updatePlayerState(IInputHandler* inputHandler) {
    bool wasDamaged = player.isDamaged();
    player.update(inputHandler);

    if (!wasDamaged && player.isDamaged()) {
        handlePlayerDamaged();
    }
}

void GameSession::updateCollisions() {
    checkPhysicsCollision();
    checkAttackCollision();
    checkDefendCollision();
    checkDamageCollision();
}

void GameSession::checkPhysicsCollision() {
    if (player.isAttachedToBuilding()) return;

    PlayerPosition pos = getPlayerPosition();
    Building* building = findBuildingForPhysics(pos);

    if (!building) return;
    if (!canAttachToBuilding(building, pos.topY)) return;

    executePhysicsAttachment(building);
}

Building* GameSession::findBuildingForPhysics(const PlayerPosition& pos) {
    return buildingManager.getBuildingAbove(
        pos.x,
        pos.topY,
        PHYSICS_COLLISION_RANGE
    );
}

void GameSession::executePhysicsAttachment(Building* building) {
    player.handlePhysicsCollision(building->getBottomY());
    player.attachToBuilding(building);
}

bool GameSession::canAttachToBuilding(Building* building, float playerTopY) const {
    bool isMovingUpward = player.getVelocityY() < UPWARD_VELOCITY_THRESHOLD;
    bool isCloseEnough = playerTopY <= building->getBottomY() + PHYSICS_COLLISION_THRESHOLD;
    return isMovingUpward && isCloseEnough;
}

void GameSession::checkAttackCollision() {
    if (hitThisFrame) return;
    if (player.getAction() != PlayerActionType::ATTACK) return;
    if (!player.isAttackActiveFrame()) return;

    PlayerPosition pos = getPlayerPosition();
    Building* building = findBuildingForAttack(pos);

    if (!building) return;
    if (!canAttackBuilding(building)) return;

    hitThisFrame = true;
    executeAttack(building);
}

Building* GameSession::findBuildingForAttack(const PlayerPosition& pos) {
    return buildingManager.getBuildingInRange(
        pos.x,
        pos.topY,
        Player::ATTACK_RANGE
    );
}

void GameSession::executeAttack(Building* building) {
    building->removeBottomFloor();
    handleAttackHit();
    detachPlayerIfAttached();
}

bool GameSession::canAttackBuilding(Building* building) const {
    return building->getBottomY() <= GameConfig::MAP_GROUND_Y;
}

void GameSession::checkDefendCollision() {
    if (hitThisFrame) return;
    if (player.getAction() != PlayerActionType::DEFEND) return;

    PlayerPosition pos = getPlayerPosition();
    Building* building = findBuildingForDefend(pos);

    if (!building) return;

    executeDefend(building);
}

Building* GameSession::findBuildingForDefend(const PlayerPosition& pos) {
    return buildingManager.getBuildingInRange(
        pos.x,
        pos.topY,
        Player::DEFENSE_RANGE
    );
}

void GameSession::executeDefend(Building* building) {
    building->applyRebound();
    handleDefenseSuccess();
    detachPlayerIfAttached();
}

void GameSession::checkDamageCollision() {
    if (hitThisFrame) return;
    if (player.isDamaged()) return;
    if (player.isJumping() || player.isAttachedToBuilding()) return;

    PlayerPosition pos = getPlayerPosition();
    Building* building = findBuildingForDamage(pos);

    if (!building) return;
    if (!canDamagePlayer(building)) return;

    executeDamage(building);
}

Building* GameSession::findBuildingForDamage(const PlayerPosition& pos) {
    return buildingManager.getBuildingAbovePlayer(
        pos.x,
        pos.topY,
        DAMAGE_COLLISION_RANGE
    );
}

void GameSession::executeDamage(Building* building) {
    player.takeDamage();
    building->applyRebound();
    handlePlayerDamaged();
}

bool GameSession::canDamagePlayer(Building* building) const {
    return building->getVelocityY() > FALLING_VELOCITY_THRESHOLD;
}

void GameSession::detachPlayerIfAttached() {
    if (player.isAttachedToBuilding()) {
        player.detachFromBuilding();
    }
}

GameSession::PlayerPosition GameSession::getPlayerPosition() const {
    int x = player.getX();
    float y = player.getY();
    return {x, y, y - Player::HEIGHT};
}

void GameSession::handleAttackHit() {
    stats.addScore(GameConfig::SCORE_PER_ATTACK_HIT);
    stats.addCombo();
    stats.addGauge(ATTACK_GAUGE_REWARD);
    messageQueue.push(MessageType::ATTACK_HIT, 1);
}

void GameSession::handleDefenseSuccess() {
    messageQueue.push(MessageType::DEFENSE_SUCCESS);
}

void GameSession::handlePlayerDamaged() {
    stats.decreaseLife();
    stats.resetCombo();
    messageQueue.push(MessageType::PLAYER_DAMAGED);
}

bool GameSession::isGameOver() const {
    return !stats.isAlive();
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
    return stats.getScore();
}

int GameSession::getCombo() const {
    return stats.getCombo();
}

int GameSession::getGauge() const {
    return stats.getGauge();
}

int GameSession::getLife() const {
    return stats.getLife();
}

int GameSession::getMaxCombo() const {
    return stats.getMaxCombo();
}

int GameSession::getPlayTimeSeconds() const {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
    return static_cast<int>(duration.count());
}

GameOverDisplayData GameSession::getGameOverData(int currentHighScore) const {
    return {
        stats.getScore(),
        stats.getMaxCombo(),
        getPlayTimeSeconds(),
        currentHighScore,
        stats.getScore() > currentHighScore
    };
}
