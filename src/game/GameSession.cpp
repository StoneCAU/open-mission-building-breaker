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
    checkCollisions();
    buildingManager.updateAll();
}

void GameSession::checkCollisions() {
    auto& buildings = buildingManager.getAll();
    const int playerX = player.getX();
    const float playerY = player.getY();
    const bool isOnGround = (playerY >= GameConfig::MAP_GROUND_Y - 0.1f);

    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        const float buildingTopY = b.getY() - b.getHeight() + 1.0f;
        const float buildingBottomY = b.getY();
        const bool withinX = (playerX >= b.getX() && playerX < b.getX() + GameConfig::BUILDING_WIDTH);

        /** [A] 점프 중 건물 밑면 충돌 체크 **/
        if (player.isJumping() && withinX) {
            const float headY = playerY - 1.0f;

            if (headY < buildingBottomY + 1.0f && playerY < buildingBottomY + 2.0f) {
                float targetY = buildingBottomY + 1.0f;
                player.forceFall(targetY);

                // 🔥 땅에 닿으면 데미지 + 건물 튕겨냄
                if (targetY >= GameConfig::MAP_GROUND_Y - 0.1f) {
                    if (!player.isInvincible()) {
                        player.takeDamage();
                        decreaseLife();
                        resetCombo();
                    }
                    b.rebound();
                }
                continue;
            }
        }

        /** [B] 몸체 충돌 **/
        if (!withinX) continue;
        if (!(playerY >= buildingTopY && playerY <= buildingBottomY)) continue;

        // 공중에서 피격 무시
        if (!isOnGround) continue;

        // ===== 무적 상태 =====
        if (player.isInvincible()) {
            b.rebound();
            break;
        }

        // ===== 공격 =====
        if (player.getAction() == PlayerAction::ATTACK) {
            b.takeHit();
            addScore(100);
            addCombo();
            break;
        }

        // ===== 방어 =====
        if (player.getAction() == PlayerAction::DEFEND) {
            b.rebound();
            break;
        }

        // ===== 지상 피격 =====
        player.takeDamage();
        decreaseLife();
        resetCombo();
        b.rebound();
        break;
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
