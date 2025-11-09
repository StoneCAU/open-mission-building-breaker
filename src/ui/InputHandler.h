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
    static InputKey getCombinedInput();
    static InputKey getDirectionalInput();
    static InputKey getGeneralInput();
};
