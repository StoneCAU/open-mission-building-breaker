#include "InputHandler.h"
#include <windows.h>

// ===== 내부 헬퍼 =====
namespace {
    bool isKeyPressed(int vkCode) {
        return (GetAsyncKeyState(vkCode) & 0x8000) != 0;
    }

    bool isCharPressed(char key) {
        return (GetAsyncKeyState(static_cast<int>(key)) & 0x8000) != 0;
    }
}

bool InputHandler::isKeyReleased(int vkCode) {
    return (GetAsyncKeyState(vkCode) & 0x8000) == 0;
}

InputKey InputHandler::getInput() {
    // 순서: 복합 → 단일 → 일반키
    InputKey key = getCombinedInput();
    if (key != InputKey::NONE) {
        return key;
    }

    key = getDirectionalInput();
    if (key != InputKey::NONE) {
        return key;
    }

    return getGeneralInput();
}

// ===== 복합 입력 =====
InputKey InputHandler::getCombinedInput() {
    const bool left = isKeyPressed(VK_LEFT);
    const bool right = isKeyPressed(VK_RIGHT);
    const bool up = isKeyPressed(VK_UP);

    if (left && up) {
        return InputKey::MOVE_LEFT_JUMP;
    }

    if (right && up) {
        return InputKey::MOVE_RIGHT_JUMP;
    }

    return InputKey::NONE;
}

// ===== 단일 방향 입력 =====
InputKey InputHandler::getDirectionalInput() {
    if (isKeyPressed(VK_UP)) {
        return InputKey::JUMP;
    }

    if (isKeyPressed(VK_DOWN)) {
        return InputKey::DEFEND;
    }

    if (isKeyPressed(VK_LEFT)) {
        return InputKey::LEFT;
    }

    if (isKeyPressed(VK_RIGHT)) {
        return InputKey::RIGHT;
    }

    return InputKey::NONE;
}

// ===== 일반 키 입력 =====
InputKey InputHandler::getGeneralInput() {
    if (isCharPressed('Z')) {
        return InputKey::ATTACK;
    }

    if (isCharPressed('X')) {
        return InputKey::ULTIMATE;
    }

    if (isCharPressed('Q')) {
        return InputKey::QUIT;
    }

    if (isKeyPressed(VK_RETURN)) {
        return InputKey::ENTER;
    }

    if (isCharPressed('R')) {
        return InputKey::RESTART;
    }

    return InputKey::NONE;
}
