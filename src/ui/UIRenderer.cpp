#include "UIRenderer.h"
#include <iostream>
#include <windows.h>
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

/** ===================== 공통 유틸 ===================== **/
void UIRenderer::clearScreen() const {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void UIRenderer::printBorder() const {
    std::cout << BORDER << NEW_LINE;
}

/** ===================== 메뉴 렌더링 ===================== **/
void UIRenderer::renderMenu(int highScore) const {
    printBorder();
    std::cout << TITLE << NEW_LINE;
    printBorder();

    std::cout << MENU_TITLE << highScore << UNIT_POINT << NEW_LINE << NEW_LINE;
    std::cout << MENU_ENTER << NEW_LINE;
    std::cout << MENU_QUIT << NEW_LINE << NEW_LINE;

    printBorder();
}

/** ===================== HUD 렌더링 ===================== **/
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

    for (int i = 0; i < s.getLife(); ++i) {
        std::cout << HUD_LIFE_ICON << SPACE;
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
        const int height  = b.getHeight();
        const int x       = b.getX();
        const auto& lines = b.getRenderLines();

        // bottom부터 위로 쌓되, 화면 범위만 체크
        for (int i = 0; i < height; ++i) {
            int drawY = bottomY - i;   // 0(위) ~ MAP_GROUND_Y(아래)
            if (drawY < 0 || drawY > GameConfig::MAP_GROUND_Y) continue;

            // lines는 위->아래 순서라면, 위에서 i칸 떨어진 줄 = lines[height-1-i]
            const std::string& blockLine = lines[height - 1 - i];
            for (int j = 0; j < (int)blockLine.size() && (GameConfig::MAP_MIN_X + x + j) <= GameConfig::MAP_MAX_X; ++j) {
                if (blockLine[j] != ' ') {
                    screen[drawY][x + j] = blockLine[j];
                }
            }
        }
    }
}

/** ===================== 플레이어 합성 ===================== **/
void UIRenderer::composePlayer(const Player& p, std::vector<std::string>& screen) const {
    const int playerX = p.getX();
    const int playerY = static_cast<int>(p.getY());

    std::string motion = "@";
    if (p.getAction() == PlayerAction::ATTACK) motion += "⚔️";
    if (p.getAction() == PlayerAction::DEFEND) motion += "🛡️";

    if (playerY < 0 || playerY > GameConfig::MAP_GROUND_Y) return;

    for (size_t i = 0; i < motion.size() && (playerX + (int)i) <= GameConfig::MAP_MAX_X; ++i) {
        screen[playerY][playerX + (int)i] = motion[i];
    }
}


/** ===================== 가이드 렌더링 ===================== **/
void UIRenderer::renderGuide() const {
    std::cout << UNDERLINE << NEW_LINE;
    std::cout << CONTROL_GUIDE << NEW_LINE;
}

/** ===================== 전체 플레이화면 ===================== **/
void UIRenderer::renderPlaying(const GameSession& s) const {
    clearScreen();

    // 1) HUD
    renderHUD(s);

    // 2) 맵 화면 버퍼 준비 (단 한 번만 출력할 바디)
    std::vector<std::string> screen(
        GameConfig::MAP_GROUND_Y + 1,
        std::string(GameConfig::MAP_MAX_X + 1, ' ')
    );

    // 3) 빌딩/플레이어를 같은 버퍼에 합성
    composeBuildings(s, screen);
    composePlayer(s.getPlayer(), screen);

    // 4) 한 번만 출력
    for (int y = 0; y <= GameConfig::MAP_GROUND_Y; ++y) {
        std::cout << screen[y] << "\n";
    }

    // 5) 가이드
    renderGuide();

    std::cout.flush();
}
