#pragma once

class ScoreManager {
public:
    ScoreManager();
    
    int loadHighScore();
    void saveHighScore(int score);
    bool isNewRecord(int score) const;

private:
    static constexpr const char* SCORE_FILE_NAME = "highscore.txt";
    static constexpr int DEFAULT_HIGH_SCORE = 0;
    
    int currentHighScore;
    
    bool fileExists() const;
    int readScoreFromFile() const;
    void writeScoreToFile(int score) const;
};
