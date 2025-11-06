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
    RESTART
};

class InputHandler {
public:
    static InputKey getInput();
};
