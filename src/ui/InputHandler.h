#pragma once

enum class InputKey {
    NONE,
    LEFT,
    RIGHT,
    ATTACK,
    DEFEND,
    JUMP,
    ULTIMATE,
    ENTER,
    QUIT,
    RESTART,
    MOVE_LEFT_JUMP,
    MOVE_RIGHT_JUMP
};

class InputHandler {
public:
    static InputKey getInput();
    static bool isKeyReleased(int vkCode);

private:
    static constexpr int KEY_PRESSED_MASK = 0x8000;

    static InputKey getCombinedInput();
    static InputKey getDirectionalInput();
    static InputKey getGeneralInput();

    static bool isVirtualKeyPressed(int vkCode);
    static bool isCharKeyPressed(char key);
    static bool isVirtualKeyReleased(int vkCode);
};
