#include "GameStats.h"
#include "GameConfig.h"
#include <algorithm>

GameStats::GameStats()
    : score(GameConfig::INITIAL_SCORE),
      combo(GameConfig::INITIAL_COMBO),
      gauge(GameConfig::INITIAL_GAUGE),
      life(GameConfig::INITIAL_LIFE),
      maxCombo(0) {}

void GameStats::reset() {
    score = GameConfig::INITIAL_SCORE;
    combo = GameConfig::INITIAL_COMBO;
    gauge = GameConfig::INITIAL_GAUGE;
    life = GameConfig::INITIAL_LIFE;
    maxCombo = 0;
}

void GameStats::addScore(int value) {
    score += value;
}

int GameStats::getScore() const {
    return score;
}

void GameStats::addCombo() {
    ++combo;
    if (combo > maxCombo) {
        maxCombo = combo;
    }
}

void GameStats::resetCombo() {
    combo = 0;
}

int GameStats::getCombo() const {
    return combo;
}

int GameStats::getMaxCombo() const {
    return maxCombo;
}

void GameStats::addGauge(int value) {
    gauge = std::min(100, gauge + value);
}

void GameStats::resetGauge() {
    gauge = 0;
}

int GameStats::getGauge() const {
    return gauge;
}

bool GameStats::canUseUltimate() const {
    return gauge >= GameConfig::ULTIMATE_GAUGE_COST;
}

void GameStats::decreaseLife() {
    if (life > 0) {
        --life;
    }
}

int GameStats::getLife() const {
    return life;
}

bool GameStats::isAlive() const {
    return life > 0;
}
