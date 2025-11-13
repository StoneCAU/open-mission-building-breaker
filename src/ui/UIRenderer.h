#pragma once
#include <string>
#include <vector>
#include "../core/game/GameSession.h"

class UIRenderer {
public:
    void clearScreen() const;
    void clearScreenFull() const;
    void renderMenu(int highScore) const;
    void renderPlaying(const GameSession& session) const;
    void renderGameOver(int finalScore, int maxCombo, int highScore, bool isNewRecord) const;

private:
    void printBorder() const;
    bool isInside(int x, int y) const;
    void drawLine(std::vector<std::string>& screen, int x, int y, const std::string& text) const;

    std::string getGaugeBar(int gauge) const;
    void renderHUD(const GameSession& session) const;
    void renderGuide() const;
    void renderMessage(const GameSession& session) const;

    void renderBody(const GameSession& session) const;
    void composeBuildings(const GameSession& session, std::vector<std::string>& screen) const;
    void composePlayer(const Player& p, const GameSession& s, std::vector<std::string>& screen) const;
};
