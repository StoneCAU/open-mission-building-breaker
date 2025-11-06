#include "UIRenderer.h"
#include <iostream>

namespace {
    constexpr const char* BORDER = "============================================";
    constexpr const char* TITLE = "        BUILDING BREAKER v1.0";
    constexpr const char* MENU_ENTER = "[ENTER] 게임 시작";
    constexpr const char* MENU_QUIT = "[Q] 종료";
}

void UIRenderer::printBorder() const {
    std::cout << BORDER << std::endl;
}

void UIRenderer::renderMenu(int highScore) const {
    printBorder();
    std::cout << TITLE << "\n";
    printBorder();
    std::cout << "최고 기록: " << highScore << "점\n\n";
    std::cout << MENU_ENTER << "\n";
    std::cout << MENU_QUIT << "\n";
    printBorder();
}
