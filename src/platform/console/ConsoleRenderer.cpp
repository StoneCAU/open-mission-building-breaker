#include "ConsoleRenderer.h"
#include <iostream>
#include <windows.h>
#include "../../core//ui/UIStrings.h"
#include "../../core/game/GameConfig.h"
#include "../../core/game/GameSession.h"
#include "../../core/game/GameOverDisplayData.h"
#include "../../core/player/Player.h"
#include "../../core/player/PlayerActionType.h"
#include "../../core/building/Building.h"

ConsoleRenderer::ConsoleRenderer()
    : mapMinX(GameConfig::MAP_MIN_X),
      mapMaxX(GameConfig::MAP_MAX_X),
      mapGroundY(GameConfig::MAP_GROUND_Y) {
    initializeActionIcons();
}

void ConsoleRenderer::initializeActionIcons() {
    actionIcons[PlayerActionType::IDLE] = "";
    actionIcons[PlayerActionType::ATTACK] = UIStrings::ICON_ATTACK;
    actionIcons[PlayerActionType::DEFEND] = UIStrings::ICON_DEFEND;
    actionIcons[PlayerActionType::ULTIMATE] = UIStrings::ICON_ATTACK;
}

void ConsoleRenderer::clearScreen() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ConsoleRenderer::clearScreenFull() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

void ConsoleRenderer::printBorder() {
    std::cout << UIStrings::BORDER << "\n";
}

void ConsoleRenderer::printEmptyLine() {
    std::cout << "\n";
}

void ConsoleRenderer::printLine(const std::string& text) {
    std::cout << text << "\n";
}

void ConsoleRenderer::flushOutput() {
    std::cout.flush();
}

bool ConsoleRenderer::isInsideMap(int x, int y) {
    return x >= mapMinX && x <= mapMaxX && y >= 0 && y <= mapGroundY;
}

bool ConsoleRenderer::isWithinDrawBounds(int x) {
    return x < mapMaxX - ICON_DRAW_MARGIN;
}

void ConsoleRenderer::drawCharacter(std::vector<std::string>& screen, int x, int y, char ch) {
    if (!isInsideMap(x, y)) return;
    if (ch == ' ') return;
    screen[y][x] = ch;
}

void ConsoleRenderer::drawLine(std::vector<std::string>& screen, int x, int y, const std::string& text) {
    if (!isInsideMap(x, y)) return;
    for (size_t j = 0; j < text.size(); ++j) {
        int drawX = x + static_cast<int>(j);
        if (!isWithinDrawBounds(drawX)) break;
        drawCharacter(screen, drawX, y, text[j]);
    }
}

void ConsoleRenderer::renderMenu(int highScore) {
    printBorder();
    printLine(UIStrings::TITLE);
    printBorder();
    printLine(UIStrings::MENU_TITLE + std::to_string(highScore) + UIStrings::UNIT_POINT);
    printEmptyLine();
    printLine(UIStrings::MENU_ENTER);
    printLine(UIStrings::MENU_QUIT);
    printEmptyLine();
    printBorder();
}

std::string ConsoleRenderer::buildGaugeBar(int gauge) {
    int filled = (gauge * GAUGE_BAR_LENGTH) / 100;
    std::string bar;
    bar.reserve(GAUGE_BAR_LENGTH);
    for (int i = 0; i < GAUGE_BAR_LENGTH; ++i)
        bar += (i < filled) ? UIStrings::HUD_GAUGE_ICON_FILLED : UIStrings::HUD_GAUGE_ICON_EMPTY;
    return bar;
}

std::string ConsoleRenderer::buildLifeDisplay(int currentLife) {
    std::string display;
    for (int i = 0; i < currentLife; ++i) {
        display += UIStrings::HUD_LIFE_ICON_FILLED;
        display += " ";
    }
    for (int i = currentLife; i < MAX_LIFE; ++i) {
        display += UIStrings::HUD_LIFE_ICON_EMPTY;
        display += " ";
    }
    return display;
}

void ConsoleRenderer::renderHUD(const GameSession& session) {
    printBorder();
    std::string hudLine = UIStrings::HUD_SCORE + std::to_string(session.getScore()) +
                          UIStrings::SEPARATOR + UIStrings::HUD_COMBO + std::to_string(session.getCombo()) +
                          UIStrings::SEPARATOR + UIStrings::HUD_GAUGE + buildGaugeBar(session.getGauge()) +
                          " " + std::to_string(session.getGauge()) + UIStrings::HUD_GAUGE_UNIT +
                          UIStrings::SEPARATOR + buildLifeDisplay(session.getLife());
    printLine(hudLine);
    printBorder();
    printEmptyLine();
    printEmptyLine();
}

void ConsoleRenderer::composeBuildingFloor(const Building& building, int floorIndex, std::vector<std::string>& screen) {
    int drawY = building.getY() - floorIndex;
    if (drawY < 0) return;
    if (drawY > mapGroundY) return;
    const auto& lines = building.getRenderLines();
    if (floorIndex >= lines.size()) return;
    drawLine(screen, building.getX(), drawY, lines[floorIndex]);
}

void ConsoleRenderer::composeBuilding(const Building& building, std::vector<std::string>& screen) {
    if (building.isDestroyed()) return;
    for (int i = 0; i < building.getHeight(); ++i)
        composeBuildingFloor(building, i, screen);
}

void ConsoleRenderer::composeBuildings(const GameSession& session, std::vector<std::string>& screen) {
    const auto& buildings = session.getBuildingManager().getAll();
    for (const auto& building : buildings)
        composeBuilding(building, screen);
}

std::string ConsoleRenderer::buildActionIcon(const Player& player) {
    auto it = actionIcons.find(player.getAction());
    if (it == actionIcons.end()) return "";
    return it->second;
}

std::string ConsoleRenderer::buildPlayerIcon(const Player& player) {
    if (player.isDamaged()) return UIStrings::ICON_DAMAGED;
    return UIStrings::ICON_PLAYER + buildActionIcon(player);
}

void ConsoleRenderer::composePlayer(const Player& player, std::vector<std::string>& screen) {
    const int px = player.getX();
    const int py = static_cast<int>(player.getY());
    if (!isInsideMap(px, py)) return;
    drawLine(screen, px, py, buildPlayerIcon(player));
}

void ConsoleRenderer::renderBody(const GameSession& session) {
    std::vector<std::string> screen(mapGroundY + 1, std::string(mapMaxX + 1, ' '));
    composeBuildings(session, screen);
    composePlayer(session.getPlayer(), screen);
    for (int y = 0; y <= mapGroundY; ++y)
        printLine(screen[y]);
}

void ConsoleRenderer::renderGuide() {
    printLine(UIStrings::UNDERLINE);
    printLine(UIStrings::CONTROL_GUIDE);
}

void ConsoleRenderer::renderMessage(const GameSession& session) {
    if (!session.messageQueue.hasMessage()) return;
    printLine(session.messageQueue.getMessage());
}

void ConsoleRenderer::renderPlaying(const GameSession& session) {
    clearScreen();
    renderHUD(session);
    renderBody(session);
    renderGuide();
    renderMessage(session);
    flushOutput();
}

std::string ConsoleRenderer::buildGameTimeDisplay(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return std::to_string(minutes) + UIStrings::UNIT_MINUTE +
           std::to_string(seconds) + UIStrings::UNIT_SECOND;
}

void ConsoleRenderer::renderGameOver(const GameOverDisplayData& data) {
    clearScreenFull();
    printBorder();
    printLine(UIStrings::GAMEOVER_TITLE);
    printBorder();
    printEmptyLine();
    printLine(UIStrings::GAMEOVER_FINAL_SCORE + std::to_string(data.finalScore) + UIStrings::UNIT_POINT);
    printLine(UIStrings::GAMEOVER_MAX_COMBO + std::to_string(data.maxCombo));
    printLine(UIStrings::GAMEOVER_PLAY_TIME + buildGameTimeDisplay(data.playTimeSeconds));
    printEmptyLine();
    if (data.isNewRecord) {
        printLine(UIStrings::GAMEOVER_NEW_RECORD);
        if (data.highScore > 0)
            printLine(UIStrings::GAMEOVER_PREV_RECORD + std::to_string(data.highScore) + UIStrings::UNIT_POINT);
        printEmptyLine();
    }
    printBorder();
    printLine(UIStrings::GAMEOVER_RESTART);
    printLine(UIStrings::MENU_QUIT);
    printBorder();
}

void ConsoleRenderer::handleMenuInput(InputKey key) {
}

void ConsoleRenderer::handleGameOverInput(InputKey key) {
}
