#include "GameSession.h"

#include "GameConfig.h"

GameSession::GameSession()
    : score(INITIAL_SCORE),
      combo(INITIAL_COMBO),
      gauge(INITIAL_GAUGE),
      life(INITIAL_LIFE) {}

void GameSession::start() {
    reset();
    buildingManager.initBuildings();
}

void GameSession::reset() {
    score = INITIAL_SCORE;
    combo = INITIAL_COMBO;
    gauge = INITIAL_GAUGE;
    life = INITIAL_LIFE;
}

void GameSession::handleInput(InputKey key) {
    player.handleInput(key);
}

void GameSession::update() {
    player.update();
    buildingManager.updateAll();
    checkCollisions();
}

void GameSession::checkCollisions() {
    auto& buildings = buildingManager.getAll();
    const int playerX = player.getX();
    const float playerY = player.getY();
    const bool isOnGround = (playerY >= GameConfig::MAP_GROUND_Y);

    for (auto& b : buildings) {
        if (b.isDestroyed()) {
            continue;
        }

        if (!b.collidesWith(playerX, playerY)) {
            continue;
        }

        // ===== [1] 공격: 건물 바로 밑에서 부수기 가능 =====
        if (player.getAction() == PlayerAction::ATTACK) {
            b.takeHit();
            addScore(100);
            addCombo();
            continue;
        }

        // ===== [4] 방어: 공중/지상 모두 위로 튕겨남 =====
        if (player.getAction() == PlayerAction::DEFEND) {
            b.rebound();
            continue;
        }

        // ===== [2] 지상 충돌: 캐릭터 생명 깎이고 건물 한 칸 튕김 =====
        if (isOnGround) {
            player.takeDamage();
            decreaseLife();
            resetCombo();
            b.rebound();
            continue;
        }

        // ===== [3] 공중 충돌: 캐릭터 막힘 (생명만 깎임) =====
        {
            resetCombo();
        }
    }
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
    if (life > 0) --life;
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
