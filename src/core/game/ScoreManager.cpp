#include "ScoreManager.h"
#include <fstream>
#include <filesystem>

ScoreManager::ScoreManager()
    : currentHighScore(DEFAULT_HIGH_SCORE) {
    currentHighScore = loadHighScore();
}

int ScoreManager::loadHighScore() {
    if (!fileExists()) {
        return DEFAULT_HIGH_SCORE;
    }
    
    int score = readScoreFromFile();
    currentHighScore = score;
    return score;
}

void ScoreManager::saveHighScore(int score) {
    if (score <= currentHighScore) {
        return;
    }
    
    writeScoreToFile(score);
    currentHighScore = score;
}

bool ScoreManager::isNewRecord(int score) const {
    return score > currentHighScore;
}

bool ScoreManager::fileExists() const {
    std::ifstream file(SCORE_FILE_NAME);
    return file.good();
}

int ScoreManager::readScoreFromFile() const {
    std::ifstream file(SCORE_FILE_NAME);
    
    if (!file.is_open()) {
        return DEFAULT_HIGH_SCORE;
    }
    
    int score;
    file >> score;
    
    if (file.fail()) {
        return DEFAULT_HIGH_SCORE;
    }
    
    return score;
}

void ScoreManager::writeScoreToFile(int score) const {
    std::ofstream file(SCORE_FILE_NAME);
    
    if (!file.is_open()) {
        return;
    }
    
    file << score;
}
