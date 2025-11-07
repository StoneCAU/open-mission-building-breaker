#include "UIRenderer.h"
#include <iostream>
#include "../game/GameConfig.h"

namespace {
    // ====== 공통 UI 상수 ======
    constexpr const char* BORDER = "==================================================";
    constexpr const char* TITLE = "        BUILDING BREAKER v1.0";
    constexpr const char* SEPARATOR = " | ";

    // ====== HUD 관련 ======
    constexpr const char* HUD_SCORE = "점수: ";
    constexpr const char* HUD_COMBO = "콤보: x";
    constexpr const char* HUD_GAUGE = "게이지: ";
    constexpr const char* HUD_GAUGE_ICON_FILLED = "█";
    constexpr const char* HUD_GAUGE_ICON_EMPTY = "░";
    constexpr const char* HUD_GAUGE_UNIT = "%";
    constexpr const char* HUD_LIFE_ICON = "❤";

    // ====== 단위 및 구분자 ======
    constexpr const char* UNIT_POINT = "점";
    constexpr const char* SPACE = " ";
    constexpr const char* NEW_LINE = "\n";

    // ====== 캐릭터 관련 아이콘 ======
    constexpr const char* ICON_PLAYER = "@";
    constexpr const char* ICON_ATTACK = "⚔️";
    constexpr const char* ICON_DEFEND = "🛡️";

    // ====== 메뉴 관련 ======
    constexpr const char* MENU_TITLE = "최고 기록: ";
    constexpr const char* MENU_ENTER = "[ENTER] 게임 시작";
    constexpr const char* MENU_QUIT = "[Q] 종료";

    // ====== 하단 가이드 ======
    constexpr const char* UNDERLINE = "_________________________________________________";
    constexpr const char* CONTROL_GUIDE = "조작: [←→]이동 [Z]공격 [↓]방어 [↑]점프 [X]필살기";
}

void UIRenderer::printBorder() const {
    std::cout << BORDER << NEW_LINE;
}

void UIRenderer::renderMenu(int highScore) const {
    printBorder();
    std::cout << TITLE << NEW_LINE;
    printBorder();

    std::cout << MENU_TITLE << highScore << UNIT_POINT << NEW_LINE << NEW_LINE;
    std::cout << MENU_ENTER << NEW_LINE;
    std::cout << MENU_QUIT << NEW_LINE << NEW_LINE;

    printBorder();
}

std::string UIRenderer::getGaugeBar(int gauge) const {
    constexpr int BAR_LENGTH = 10;
    int filled = (gauge * BAR_LENGTH) / 100;
    std::string bar;
    bar.reserve(BAR_LENGTH);

    for (int i = 0; i < BAR_LENGTH; ++i) {
        if (i < filled) {
            bar += HUD_GAUGE_ICON_FILLED;
        }
        if (i >= filled) {
            bar += HUD_GAUGE_ICON_EMPTY;
        }
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

    for (int i = 0; i < s.getLife(); ++i) {
        std::cout << HUD_LIFE_ICON << SPACE;
    }

    std::cout << NEW_LINE;
    printBorder();
    std::cout << NEW_LINE << NEW_LINE;
}

void UIRenderer::renderPlayer(const Player& p) const {
    const int playerX = p.getX();
    const int playerY = p.getY();

    std::string motion = getPlayerMotion(p);

    for (int y = 0; y <= GameConfig::MAP_GROUND_Y; ++y) {
        if (y == playerY) {
            renderPlayerLine(playerX, motion);
            continue;
        }

        renderEmptyLine();
    }
}

std::string UIRenderer::getPlayerMotion(const Player& p) const {
    std::string motion = ICON_PLAYER;

    if (p.getAction() == PlayerAction::ATTACK) {
        motion += ICON_ATTACK;
    }

    if (p.getAction() == PlayerAction::DEFEND) {
        motion += ICON_DEFEND;
    }

    return motion;
}

void UIRenderer::renderPlayerLine(int playerX, const std::string& motion) const {
    for (int x = GameConfig::MAP_MIN_X; x <= GameConfig::MAP_MAX_X; ++x) {
        if (x == playerX) {
            std::cout << motion;
        }

        if (x != playerX) {
            std::cout << SPACE;
        }
    }
    std::cout << NEW_LINE;
}

void UIRenderer::renderEmptyLine() const {
    for (int x = GameConfig::MAP_MIN_X; x <= GameConfig::MAP_MAX_X; ++x) {
        std::cout << SPACE;
    }
    std::cout << NEW_LINE;
}


void UIRenderer::renderGuide() const {
    std::cout << UNDERLINE << NEW_LINE;
    std::cout << CONTROL_GUIDE << NEW_LINE;
}

void UIRenderer::renderPlaying(const GameSession& s) const {
    renderHUD(s);
    renderPlayer(s.getPlayer());
    renderGuide();
}
