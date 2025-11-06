#include "InputHandler.h"
#include <conio.h>

InputKey InputHandler::getInput() {
    if (!_kbhit()) {
        return InputKey::NONE;
    }

    char input = _getch();

    // 특수 키(2바이트) 감지 조건문
    if (input == 0 || input == -32) {
        input = _getch();
        if (input == 75) return InputKey::LEFT;   // ←
        if (input == 77) return InputKey::RIGHT;  // →
        if (input == 72) return InputKey::JUMP;   // ↑
        if (input == 80) return InputKey::DEFEND; // ↓
    }

    // 일반 키
    if (input == 'z' || input == 'Z') return InputKey::ATTACK;
    if (input == 'x' || input == 'X') return InputKey::ULTIMATE;
    if (input == 'q' || input == 'Q') return InputKey::QUIT;
    if (input == '\r') return InputKey::ENTER;  // Enter
    if (input == 'r' || input == 'R') return InputKey::RESTART;

    return InputKey::NONE;
}