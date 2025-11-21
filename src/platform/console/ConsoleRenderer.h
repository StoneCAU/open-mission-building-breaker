#pragma once

#include "../../interfaces/IRenderer.h"
#include "../../core/player/Player.h"
#include "../../core/player/PlayerActionType.h"
#include <vector>
#include <string>
#include <map>

class ConsoleRenderer : public IRenderer {
public:
    ConsoleRenderer();

    void renderMenu(int highScore) override;
    void renderPlaying(const GameSession& session) override;
    void renderGameOver(const GameOverDisplayData& data) override;
    void clearScreen() override;
    void clearScreenFull() override;
    void flushOutput() override;

    void handleMenuInput(InputKey key) override;
    void handleGameOverInput(InputKey key) override;

private:
    static constexpr int GAUGE_BAR_LENGTH = 10;
    static constexpr int MAX_LIFE = 3;
    static constexpr int ICON_DRAW_MARGIN = 2;

    int mapMinX;
    int mapMaxX;
    int mapGroundY;

    std::map<PlayerActionType, std::string> actionIcons;

    void initializeActionIcons();
    void renderHUD(const GameSession& session);
    void renderBody(const GameSession& session);
    void renderGuide();
    void renderMessage(const GameSession& session);
    void composeBuildings(const GameSession& session, std::vector<std::string>& screen);
    void composePlayer(const Player& player, std::vector<std::string>& screen);
    void composeBuilding(const Building& building, std::vector<std::string>& screen);
    void composeBuildingFloor(const Building& building, int floorIndex, std::vector<std::string>& screen);
    std::string buildGaugeBar(int gauge);
    std::string buildLifeDisplay(int currentLife);
    std::string buildPlayerIcon(const Player& player);
    std::string buildActionIcon(const Player& player);
    std::string buildGameTimeDisplay(int totalSeconds);
    void drawLine(std::vector<std::string>& screen, int x, int y, const std::string& text);
    void drawCharacter(std::vector<std::string>& screen, int x, int y, char ch);
    bool isInsideMap(int x, int y);
    bool isWithinDrawBounds(int x);
    void printBorder();
    void printEmptyLine();
    void printLine(const std::string& text);
};
