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

            if (playerY <= buildingBottomY + 3.0f && headY <= buildingBottomY + 2.0f) {
                float targetY = buildingBottomY + 1.0f;
                player.forceFall(targetY);

                if (targetY >= GameConfig::MAP_GROUND_Y - 0.1f) {
                    if (player.getAction() == PlayerAction::DEFEND || player.isInvincible()) {
                        b.rebound();
                    } else {
                        player.takeDamage();
                        decreaseLife();
                        resetCombo();
                        b.rebound();
                    }
                }
                continue;
            }
        }

        /** [A-2] 🔥 점프 시작 직후 건물 범위 안에 있으면 피격 **/
        if (player.isJumping() && withinX && isOnGround) {
            // 점프 막 시작했는데 건물 안에 있음 = 꼼수
            if (playerY >= buildingTopY - 1.0f && playerY <= buildingBottomY) {
                // 방어 중이면 리바운드
                if (player.getAction() == PlayerAction::DEFEND || player.isInvincible()) {
                    b.rebound();
                    player.forceFall(GameConfig::MAP_GROUND_Y);
                } else {
                    // 피격
                    player.takeDamage();
                    decreaseLife();
                    resetCombo();
                    b.rebound();
                    player.forceFall(GameConfig::MAP_GROUND_Y);
                }
                break;
            }
        }

        /** [B] 몸체 충돌 **/
        if (!withinX) continue;
        if (!(playerY >= buildingTopY && playerY <= buildingBottomY)) continue;

        // 공중 충돌
        if (!isOnGround) {
            if (player.isInvincible()) {
                b.rebound();
                break;
            }
            if (player.getAction() == PlayerAction::DEFEND) {
                b.rebound();
                break;
            }
            if (player.getAction() == PlayerAction::ATTACK) {
                b.takeHit();
                addScore(100);
                addCombo();
                break;
            }
            player.forceFall(playerY);
            break;
        }

        // 지상 충돌
        if (player.isInvincible()) {
            b.rebound();
            break;
        }

        if (player.getAction() == PlayerAction::ATTACK) {
            b.takeHit();
            addScore(100);
            addCombo();
            break;
        }

        if (player.getAction() == PlayerAction::DEFEND) {
            b.rebound();
            break;
        }

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
