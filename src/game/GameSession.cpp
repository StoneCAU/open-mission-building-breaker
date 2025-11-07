#include "GameSession.h"

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
