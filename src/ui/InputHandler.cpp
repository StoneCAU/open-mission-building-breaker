#include "InputHandler.h"
#include <conio.h>
#include <windows.h>

InputKey InputHandler::getInput() {
    bool left = GetAsyncKeyState(VK_LEFT) & 0x8000;
    bool right = GetAsyncKeyState(VK_RIGHT) & 0x8000;
    bool up = GetAsyncKeyState(VK_UP) & 0x8000;
    bool down = GetAsyncKeyState(VK_DOWN) & 0x8000;

    // ← + ↑ 동시 입력
    if (left && up) {
        return InputKey::MOVE_LEFT_JUMP;
    }

    if (right && up) {
        return InputKey::MOVE_RIGHT_JUMP;
    }

    // 단일 키 입력
    if (up) {
        return InputKey::JUMP;
    }

    if (down) {
        return InputKey::DEFEND;
    }

    if (left) {
        return InputKey::LEFT;
    }

    if (right) {
        return InputKey::RIGHT;
    }

    if (GetAsyncKeyState('Z') & 0x8000) {
        return InputKey::ATTACK;
    }

    if (GetAsyncKeyState('X') & 0x8000) {
        return InputKey::ULTIMATE;
    }

    if (GetAsyncKeyState('Q') & 0x8000) {
        return InputKey::QUIT;
    }

    if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
        return InputKey::ENTER;
    }

    if (GetAsyncKeyState('R') & 0x8000) {
        return InputKey::RESTART;
    }

    return InputKey::NONE;
}
