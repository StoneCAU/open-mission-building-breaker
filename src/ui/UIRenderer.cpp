#include "UIRenderer.h"
#include <iostream>

namespace {
    // ====== 공통 UI 상수 ======
    constexpr const char* BORDER = "=====================================================";
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

    // ====== 메뉴 관련 ======
    constexpr const char* MENU_TITLE = "최고 기록: ";
    constexpr const char* MENU_ENTER = "[ENTER] 게임 시작";
    constexpr const char* MENU_QUIT = "[Q] 종료";

    // ====== 하단 가이드 ======
    constexpr const char* UNDERLINE = "_________________________________________________";
    constexpr const char* CONTROL_GUIDE ="조작: [←→]이동 [Z]공격 [↓]방어 [↑]점프 [X]필살기";
}

void UIRenderer::printBorder() const {
    std::cout << BORDER << std::endl;
}

/** ========== 메뉴 화면 ========== **/
void UIRenderer::renderMenu(int highScore) const {
    printBorder();
    std::cout << TITLE << "\n";
    printBorder();

    // 일관된 포맷으로 출력
    std::cout << MENU_TITLE << highScore << "점\n\n";
    std::cout << MENU_ENTER << "\n";
    std::cout << MENU_QUIT << "\n\n";

    printBorder();
}

/** ========== 게이지 시각화 ========== **/
std::string UIRenderer::getGaugeBar(int gauge) const {
    constexpr int BAR_LENGTH = 10;
    int filled = (gauge * BAR_LENGTH) / 100;
    std::string bar;
    bar.reserve(BAR_LENGTH);
    for (int i = 0; i < BAR_LENGTH; ++i)
        bar += (i < filled ? HUD_GAUGE_ICON_FILLED : HUD_GAUGE_ICON_EMPTY);
    return bar;
}

/** ========== 플레이 화면 ========== **/
void UIRenderer::renderPlaying(const GameSession& s) const {
    printBorder();
    std::cout << HUD_SCORE << s.getScore()
              << SEPARATOR << HUD_COMBO << s.getCombo()
              << SEPARATOR << HUD_GAUGE << getGaugeBar(s.getGauge())
              << " " << s.getGauge() << HUD_GAUGE_UNIT
              << SEPARATOR;

    for (int i = 0; i < s.getLife(); ++i)
        std::cout << HUD_LIFE_ICON << " ";

    std::cout << "\n";
    printBorder();

    // 위쪽 공백
    std::cout << "\n\n\n";

    // 필드 영역은 추후 FieldRenderer로 대체
    std::cout << "\n\n\n";

    // 아래쪽 공백
    std::cout << "\n";

    std::cout << UNDERLINE << "\n";
    std::cout << CONTROL_GUIDE << "\n";
}
