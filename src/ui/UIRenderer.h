#pragma once

#include <vector>
#include <string>
#include <map>
#include "../core/game/GameSession.h"
#include "../core/player/PlayerActionType.h"

class UIRenderer {
public:
    UIRenderer();

    void clearScreen() const;
    void clearScreenFull() const;

    void renderMenu(int highScore) const;
    void renderPlaying(const GameSession& session) const;
    void renderGameOver(const GameOverDisplayData& data) const;

private:
    static constexpr int GAUGE_BAR_LENGTH = 10;
    static constexpr int MAX_LIFE = 3;
    static constexpr int ICON_DRAW_MARGIN = 2;

    int mapMinX;
    int mapMaxX;
    int mapGroundY;

    std::map<PlayerActionType, std::string> actionIcons;

    void initializeActionIcons();

    void renderHUD(const GameSession& session) const;
    void renderBody(const GameSession& session) const;
    void renderGuide() const;
    void renderMessage(const GameSession& session) const;

    void composeBuildings(const GameSession& session, std::vector<std::string>& screen) const;
    void composePlayer(const Player& player, std::vector<std::string>& screen) const;
    void composeBuilding(const Building& building, std::vector<std::string>& screen) const;
    void composeBuildingFloor(const Building& building, int floorIndex, std::vector<std::string>& screen) const;

    std::string buildGaugeBar(int gauge) const;
    std::string buildLifeDisplay(int currentLife) const;
    std::string buildPlayerIcon(const Player& player) const;
    std::string buildActionIcon(const Player& player) const;
    std::string buildGameTimeDisplay(int totalSeconds) const;

    void drawLine(std::vector<std::string>& screen, int x, int y, const std::string& text) const;
    void drawCharacter(std::vector<std::string>& screen, int x, int y, char ch) const;

    bool isInsideMap(int x, int y) const;
    bool isWithinDrawBounds(int x) const;

    void printBorder() const;
    void printEmptyLine() const;
    void printLine(const std::string& text) const;
    void flushOutput() const;
};
