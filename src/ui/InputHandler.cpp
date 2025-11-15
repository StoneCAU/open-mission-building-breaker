#include "InputHandler.h"
#include <windows.h>

InputKey InputHandler::getInput() {
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

bool InputHandler::isKeyReleased(int vkCode) {
    return !isVirtualKeyPressed(vkCode);
}

InputKey InputHandler::getCombinedInput() {
    const bool left = isVirtualKeyPressed(VK_LEFT);
    const bool right = isVirtualKeyPressed(VK_RIGHT);
    const bool up = isVirtualKeyPressed(VK_UP);

    if (left && up) {
        return InputKey::MOVE_LEFT_JUMP;
    }

    if (right && up) {
        return InputKey::MOVE_RIGHT_JUMP;
    }

    return InputKey::NONE;
}

InputKey InputHandler::getDirectionalInput() {
    if (isVirtualKeyPressed(VK_UP)) {
        return InputKey::JUMP;
    }

    if (isVirtualKeyPressed(VK_DOWN)) {
        return InputKey::DEFEND;
    }

    if (isVirtualKeyPressed(VK_LEFT)) {
        return InputKey::LEFT;
    }

    if (isVirtualKeyPressed(VK_RIGHT)) {
        return InputKey::RIGHT;
    }

    return InputKey::NONE;
}

InputKey InputHandler::getGeneralInput() {
    if (isCharKeyPressed('Z')) {
        return InputKey::ATTACK;
    }

    if (isCharKeyPressed('X')) {
        return InputKey::ULTIMATE;
    }

    if (isCharKeyPressed('Q')) {
        return InputKey::QUIT;
    }

    if (isVirtualKeyPressed(VK_RETURN)) {
        return InputKey::ENTER;
    }

    if (isCharKeyPressed('R')) {
        return InputKey::RESTART;
    }

    return InputKey::NONE;
}

bool InputHandler::isVirtualKeyPressed(int vkCode) {
    return (GetAsyncKeyState(vkCode) & KEY_PRESSED_MASK) != 0;
}

bool InputHandler::isCharKeyPressed(char key) {
    return (GetAsyncKeyState(static_cast<int>(key)) & KEY_PRESSED_MASK) != 0;
}

bool InputHandler::isVirtualKeyReleased(int vkCode) {
    return (GetAsyncKeyState(vkCode) & KEY_PRESSED_MASK) == 0;
}
