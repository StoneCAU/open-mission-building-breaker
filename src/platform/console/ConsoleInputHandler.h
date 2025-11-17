#pragma once

#include "../../interfaces/IInputHandler.h"

class ConsoleInputHandler : public IInputHandler {
public:
    InputKey getInput() override;
    bool isKeyReleased(int vkCode) override;

private:
    static constexpr int KEY_PRESSED_MASK = 0x8000;

    InputKey getCombinedInput();
    InputKey getDirectionalInput();
    InputKey getGeneralInput();

    bool isVirtualKeyPressed(int vkCode);
    bool isCharKeyPressed(char key);
    bool isVirtualKeyReleased(int vkCode);
};
