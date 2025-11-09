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
    const float headY = playerY - 1.0f;
    const bool isOnGround = (playerY >= GameConfig::MAP_GROUND_Y - 0.1f);

    for (auto& b : buildings) {
        if (b.isDestroyed()) continue;

        const float buildingTopY = b.getY() - b.getHeight() + 1.0f;
        const float buildingBottomY = b.getY();
        const bool withinX = (playerX >= b.getX() && playerX < b.getX() + GameConfig::BUILDING_WIDTH);

        /** [A] 점프 중 건물 밑면 충돌 (대가리 박기) **/
        if (player.isJumping() && withinX) {
            // 머리가 건물 바닥에 닿으면 즉시 점프 중단하고 그 자리에서 멈춤
            if (headY <= buildingBottomY) {
                // 일단 대가리 박은 상태 유지
                player.forceFall(buildingBottomY + 1.0f);

                // ⚔️ 공격 중이면: 건물 공격 (대가리 박은 채로)
                if (player.getAction() == PlayerAction::ATTACK) {
                    b.takeHit();
                    addScore(100);
                    addCombo();
                    continue;
                }

                // 🛡️ 방어 중이면: 건물 튕기고 캐릭터 즉시 낙하
                if (player.getAction() == PlayerAction::DEFEND) {
                    b.rebound();
                    continue;
                }

                // 땅에 닿으면 피격 처리
                if (buildingBottomY + 1.0f >= GameConfig::MAP_GROUND_Y - 0.1f) {
                    if (player.isInvincible()) {
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

        /** [B] 공격/방어 범위 판정 (머리 위 한 칸) **/
        if (withinX) {
            // ⚔️ 공격 판정: 머리 위 GameConfig::PLAYER_ATTACK_RANGE 내의 빌딩 하단
            if (player.getAction() == PlayerAction::ATTACK) {
                if (buildingBottomY <= headY &&
                    buildingBottomY >= headY - GameConfig::PLAYER_ATTACK_RANGE) {
                    b.takeHit();
                    addScore(100);
                    addCombo();
                    continue;
                }
            }

            // 🛡️ 방어 판정: 머리 위 GameConfig::PLAYER_DEFENSE_RANGE 내면 반동
            if (player.getAction() == PlayerAction::DEFEND) {
                if (buildingBottomY <= headY &&
                    buildingBottomY >= headY - GameConfig::PLAYER_DEFENSE_RANGE) {
                    b.rebound();
                    continue;
                }
            }
        }

        /** [C] 몸체 충돌 **/
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
