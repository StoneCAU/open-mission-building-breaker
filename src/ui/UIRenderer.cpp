#include "UIRenderer.h"
#include <iostream>
#include <windows.h>
#include "../core/game/GameConfig.h"
#include "../core/game/GameSession.h"

namespace {
    // ====== 공통 UI 상수 ======
    constexpr const char *BORDER = "==================================================";
    constexpr const char *TITLE = "        BUILDING BREAKER v1.0";
    constexpr const char *SEPARATOR = " | ";

    // ====== HUD 관련 ======
    constexpr const char *HUD_SCORE = "점수: ";
    constexpr const char *HUD_COMBO = "콤보: x";
    constexpr const char *HUD_GAUGE = "게이지: ";
    constexpr const char *HUD_GAUGE_ICON_FILLED = "█";
    constexpr const char *HUD_GAUGE_ICON_EMPTY = "░";
    constexpr const char *HUD_GAUGE_UNIT = "%";
    constexpr const char *HUD_LIFE_ICON_FILLED = "❤";
    constexpr const char *HUD_LIFE_ICON_EMPTY = "♡";

    // ====== 단위 및 구분자 ======
    constexpr const char *UNIT_POINT = "점";
    constexpr const char *SPACE = " ";
    constexpr const char *NEW_LINE = "\n";

    // ====== 캐릭터 관련 아이콘 ======
    constexpr const char *ICON_PLAYER = "@";
    constexpr const char *ICON_ATTACK = "⚔️";
    constexpr const char *ICON_DEFEND = "🛡️";
    constexpr const char *ICON_DAMAGED = "💥";

    // ====== 메뉴 관련 ======
    constexpr const char *MENU_TITLE = "최고 기록: ";
    constexpr const char *MENU_ENTER = "[ENTER] 게임 시작";
    constexpr const char *MENU_QUIT = "[Q] 종료";

    // ====== 하단 가이드 ======
    constexpr const char *UNDERLINE = "_________________________________________________";
    constexpr const char *CONTROL_GUIDE = "조작: [←→]이동 [Z]공격 [↓]방어 [↑]점프 [X]필살기";

    // ====== 게임오버 관련 ======
    constexpr const char *GAMEOVER_TITLE = "           GAME OVER";
    constexpr const char *GAMEOVER_FINAL_SCORE = "최종 점수: ";
    constexpr const char *GAMEOVER_MAX_COMBO = "최고 콤보: x";
    constexpr const char *GAMEOVER_NEW_RECORD = "★ 새로운 최고 기록 달성! ★";
    constexpr const char *GAMEOVER_PREV_RECORD = "이전 기록: ";
    constexpr const char *GAMEOVER_RESTART = "[R] 재시작";
    constexpr const char *GAMEOVER_PLAY_TIME = "플레이 시간: ";
    constexpr const char *UNIT_MINUTE = "분 ";
    constexpr const char *UNIT_SECOND = "초";
}

/** ===================== 공통 유틸 ===================== **/
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
    std::cout << BORDER << NEW_LINE;
}

bool UIRenderer::isInside(int x, int y) const {
    return x >= GameConfig::MAP_MIN_X && x <= GameConfig::MAP_MAX_X &&
           y >= 0 && y <= GameConfig::MAP_GROUND_Y;
}

void UIRenderer::drawLine(std::vector<std::string>& screen, int x, int y, const std::string& text) const {
    if (!isInside(x, y)) return;

    for (int j = 0; j < static_cast<int>(text.size()); ++j) {
        int drawX = x + j;

        // 콘솔 경계 넘어가면 중단 (▩ 문자 깨짐 방지)
        if (drawX >= GameConfig::MAP_MAX_X - 2) break;

        const char ch = text[j];
        if (ch == *SPACE) continue;  // 공백은 건너뜀

        screen[y][drawX] = ch;
    }
}

/** ===================== 메뉴 ===================== **/
void UIRenderer::renderMenu(int highScore) const {
    printBorder();
    std::cout << TITLE << NEW_LINE;
    printBorder();
    std::cout << MENU_TITLE << highScore << UNIT_POINT << NEW_LINE << NEW_LINE;
    std::cout << MENU_ENTER << NEW_LINE;
    std::cout << MENU_QUIT << NEW_LINE << NEW_LINE;
    printBorder();
}

/** ===================== HUD ===================== **/
std::string UIRenderer::getGaugeBar(int gauge) const {
    constexpr int BAR_LENGTH = 10;
    int filled = (gauge * BAR_LENGTH) / 100;
    std::string bar;
    bar.reserve(BAR_LENGTH);

    for (int i = 0; i < BAR_LENGTH; ++i) {
        bar += (i < filled) ? HUD_GAUGE_ICON_FILLED : HUD_GAUGE_ICON_EMPTY;
    }
    return bar;
}

void UIRenderer::renderHUD(const GameSession& s) const {
    printBorder();
    std::cout << HUD_SCORE << s.getScore()
              << SEPARATOR << HUD_COMBO << s.getCombo()
              << SEPARATOR << HUD_GAUGE << getGaugeBar(s.getGauge())
              << SPACE << s.getGauge() << HUD_GAUGE_UNIT
              << SEPARATOR;

    int currentLife = s.getLife();
    int maxLife = GameConfig::INITIAL_LIFE;

    for (int i = 0; i < currentLife; ++i) {
        std::cout << HUD_LIFE_ICON_FILLED << SPACE;
    }

    for (int i = currentLife; i < maxLife; ++i) {
        std::cout << HUD_LIFE_ICON_EMPTY << SPACE;
    }

    std::cout << NEW_LINE;
    printBorder();
    std::cout << NEW_LINE << NEW_LINE;
}

/** ===================== 빌딩 합성 ===================== **/
void UIRenderer::composeBuildings(const GameSession& s, std::vector<std::string>& screen) const {
    const auto& buildings = s.getBuildingManager().getAll();

    for (const auto& b : buildings) {
        if (b.isDestroyed()) continue;

        const int bottomY = b.getY();
        const int height = b.getHeight();
        const int x = b.getX();
        const auto& lines = b.getRenderLines();

        for (int i = 0; i < height; ++i) {
            int drawY = bottomY - i;
            if (drawY < 0 || drawY > GameConfig::MAP_GROUND_Y) continue;

            const std::string& blockLine = lines[height - 1 - i];
            drawLine(screen, x, drawY, blockLine);
        }
    }
}

/** ===================== 플레이어 합성 ===================== **/
void UIRenderer::composePlayer(const Player& p, const GameSession& s, std::vector<std::string>& screen) const {
    const int px = p.getX();
    const int py = static_cast<int>(p.getY());
    if (!isInside(px, py)) return;

    std::string motion = ICON_PLAYER;

    if (p.isDamaged()) {
        motion = ICON_DAMAGED;
    }

    if (!p.isDamaged() && p.getAction() == PlayerActionType::ATTACK) {
        motion += ICON_ATTACK;
    }

    if (!p.isDamaged() && p.getAction() == PlayerActionType::DEFEND) {
        motion += ICON_DEFEND;
    }

    drawLine(screen, px, py, motion);
}


/** ===================== 본문 출력 (빌딩 + 플레이어) ===================== **/
void UIRenderer::renderBody(const GameSession& s) const {
    std::vector<std::string> screen(
        GameConfig::MAP_GROUND_Y + 1,
        std::string(GameConfig::MAP_MAX_X + 1, *SPACE)
    );

    composeBuildings(s, screen);
    composePlayer(s.getPlayer(), s, screen);

    for (int y = 0; y <= GameConfig::MAP_GROUND_Y; ++y) {
        std::cout << screen[y] << NEW_LINE;
    }
}

/** ===================== 가이드 ===================== **/
void UIRenderer::renderGuide() const {
    std::cout << UNDERLINE << NEW_LINE;
    std::cout << CONTROL_GUIDE << NEW_LINE;
}

/** ===================== 메시지 ===================== **/
void UIRenderer::renderMessage(const GameSession& session) const {
    const UIMessageQueue& msgQueue = session.messageQueue;

    if (msgQueue.hasMessage()) {
        std::cout << msgQueue.getMessage() << NEW_LINE;
    }
}


/** ===================== 전체 출력 ===================== **/
void UIRenderer::renderPlaying(const GameSession& s) const {
    clearScreen();
    renderHUD(s);
    renderBody(s);
    renderGuide();
    renderMessage(s);
    std::cout.flush();
}

void UIRenderer::renderGameOver(GameOverDisplayData data) const {
    clearScreenFull();

    printBorder();
    std::cout << GAMEOVER_TITLE << NEW_LINE;
    printBorder();
    std::cout << NEW_LINE;

    std::cout << GAMEOVER_FINAL_SCORE << data.finalScore << UNIT_POINT << NEW_LINE;
    std::cout << GAMEOVER_MAX_COMBO << data.maxCombo << NEW_LINE;

    // 플레이 시간 표시
    int minutes = data.playTimeSeconds / 60;
    int seconds = data.playTimeSeconds % 60;
    std::cout << GAMEOVER_PLAY_TIME << minutes << UNIT_MINUTE << seconds << UNIT_SECOND << NEW_LINE;
    std::cout << NEW_LINE;

    if (data.isNewRecord) {
        std::cout << GAMEOVER_NEW_RECORD << NEW_LINE;
        std::cout << GAMEOVER_PREV_RECORD << data.highScore << UNIT_POINT << NEW_LINE;
        std::cout << NEW_LINE;
    }

    printBorder();
    std::cout << GAMEOVER_RESTART << NEW_LINE;
    std::cout << MENU_QUIT << NEW_LINE;
    printBorder();
}