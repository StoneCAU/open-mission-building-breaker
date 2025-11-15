#include "UIRenderer.h"
#include <iostream>
#include <windows.h>
#include "../core/game/GameConfig.h"
#include "UIStrings.h"

UIRenderer::UIRenderer()
    : mapMinX(GameConfig::MAP_MIN_X),
      mapMaxX(GameConfig::MAP_MAX_X),
      mapGroundY(GameConfig::MAP_GROUND_Y) {
    initializeActionIcons();
}

void UIRenderer::initializeActionIcons() {
    actionIcons[PlayerActionType::IDLE] = "";
    actionIcons[PlayerActionType::ATTACK] = UIStrings::ICON_ATTACK;
    actionIcons[PlayerActionType::DEFEND] = UIStrings::ICON_DEFEND;
    actionIcons[PlayerActionType::ULTIMATE] = UIStrings::ICON_ATTACK;
}

void UIRenderer::clearScreen() const {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void UIRenderer::clearScreenFull() const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

void UIRenderer::printBorder() const {
    printLine(UIStrings::BORDER);
}

void UIRenderer::printEmptyLine() const {
    std::cout << "\n";
}

void UIRenderer::printLine(const std::string& text) const {
    std::cout << text << "\n";
}

void UIRenderer::flushOutput() const {
    std::cout.flush();
}

bool UIRenderer::isInsideMap(int x, int y) const {
    return x >= mapMinX && x <= mapMaxX && y >= 0 && y <= mapGroundY;
}

bool UIRenderer::isWithinDrawBounds(int x) const {
    return x < mapMaxX - ICON_DRAW_MARGIN;
}

void UIRenderer::drawCharacter(std::vector<std::string>& screen, int x, int y, char ch) const {
    if (!isInsideMap(x, y)) return;
    if (ch == ' ') return;

    screen[y][x] = ch;
}

void UIRenderer::drawLine(std::vector<std::string>& screen, int x, int y, const std::string& text) const {
    if (!isInsideMap(x, y)) return;

    for (size_t j = 0; j < text.size(); ++j) {
        int drawX = x + static_cast<int>(j);

        if (!isWithinDrawBounds(drawX)) break;

        drawCharacter(screen, drawX, y, text[j]);
    }
}

void UIRenderer::renderMenu(int highScore) const {
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

std::string UIRenderer::buildGaugeBar(int gauge) const {
    int filled = (gauge * GAUGE_BAR_LENGTH) / 100;
    std::string bar;
    bar.reserve(GAUGE_BAR_LENGTH);

    for (int i = 0; i < GAUGE_BAR_LENGTH; ++i) {
        bar += (i < filled) ? UIStrings::HUD_GAUGE_ICON_FILLED : UIStrings::HUD_GAUGE_ICON_EMPTY;
    }
    return bar;
}

std::string UIRenderer::buildLifeDisplay(int currentLife) const {
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

void UIRenderer::renderHUD(const GameSession& session) const {
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

void UIRenderer::composeBuildingFloor(const Building& building, int floorIndex, std::vector<std::string>& screen) const {
    int drawY = building.getY() - floorIndex;

    if (drawY < 0) return;
    if (drawY > mapGroundY) return;

    const auto& lines = building.getRenderLines();

    if (floorIndex >= lines.size()) return;

    drawLine(screen, building.getX(), drawY, lines[floorIndex]);
}

void UIRenderer::composeBuilding(const Building& building, std::vector<std::string>& screen) const {
    if (building.isDestroyed()) return;

    for (int i = 0; i < building.getHeight(); ++i) {
        composeBuildingFloor(building, i, screen);
    }
}

void UIRenderer::composeBuildings(const GameSession& session, std::vector<std::string>& screen) const {
    const auto& buildings = session.getBuildingManager().getAll();

    for (const auto& building : buildings) {
        composeBuilding(building, screen);
    }
}

std::string UIRenderer::buildActionIcon(const Player& player) const {
    auto it = actionIcons.find(player.getAction());

    if (it == actionIcons.end()) {
        return "";
    }

    return it->second;
}

std::string UIRenderer::buildPlayerIcon(const Player& player) const {
    if (player.isDamaged()) {
        return UIStrings::ICON_DAMAGED;
    }

    return UIStrings::ICON_PLAYER + buildActionIcon(player);
}

void UIRenderer::composePlayer(const Player& player, std::vector<std::string>& screen) const {
    const int px = player.getX();
    const int py = static_cast<int>(player.getY());

    if (!isInsideMap(px, py)) return;

    drawLine(screen, px, py, buildPlayerIcon(player));
}

void UIRenderer::renderBody(const GameSession& session) const {
    std::vector<std::string> screen(
        mapGroundY + 1,
        std::string(mapMaxX + 1, ' ')
    );

    composeBuildings(session, screen);
    composePlayer(session.getPlayer(), screen);

    for (int y = 0; y <= mapGroundY; ++y) {
        printLine(screen[y]);
    }
}

void UIRenderer::renderGuide() const {
    printLine(UIStrings::UNDERLINE);
    printLine(UIStrings::CONTROL_GUIDE);
}

void UIRenderer::renderMessage(const GameSession& session) const {
    if (!session.messageQueue.hasMessage()) return;

    printLine(session.messageQueue.getMessage());
}

void UIRenderer::renderPlaying(const GameSession& session) const {
    clearScreen();
    renderHUD(session);
    renderBody(session);
    renderGuide();
    renderMessage(session);
    flushOutput();
}

std::string UIRenderer::buildGameTimeDisplay(int totalSeconds) const {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    return std::to_string(minutes) + UIStrings::UNIT_MINUTE +
           std::to_string(seconds) + UIStrings::UNIT_SECOND;
}

void UIRenderer::renderGameOver(const GameOverDisplayData& data) const {
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

        if (data.highScore > 0) {
            printLine(UIStrings::GAMEOVER_PREV_RECORD + std::to_string(data.highScore) + UIStrings::UNIT_POINT);
        }

        printEmptyLine();
    }

    printBorder();
    printLine(UIStrings::GAMEOVER_RESTART);
    printLine(UIStrings::MENU_QUIT);
    printBorder();
}
