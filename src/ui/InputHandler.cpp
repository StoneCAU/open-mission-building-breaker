#include "InputHandler.h"
#include <conio.h>
#include <windows.h>

InputKey InputHandler::getInput() {
    // 방향키
    if (GetAsyncKeyState(VK_LEFT)  & 0x8000) return InputKey::LEFT;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) return InputKey::RIGHT;
    if (GetAsyncKeyState(VK_UP)    & 0x8000) return InputKey::JUMP;
    if (GetAsyncKeyState(VK_DOWN)  & 0x8000) return InputKey::DEFEND;

    // 일반키
    if (GetAsyncKeyState('Z') & 0x8000) return InputKey::ATTACK;
    if (GetAsyncKeyState('X') & 0x8000) return InputKey::ULTIMATE;
    if (GetAsyncKeyState('R') & 0x8000) return InputKey::RESTART;
    if (GetAsyncKeyState('Q') & 0x8000) return InputKey::QUIT;
    if (GetAsyncKeyState(VK_RETURN) & 0x8000) return InputKey::ENTER;

    return InputKey::NONE;
}
