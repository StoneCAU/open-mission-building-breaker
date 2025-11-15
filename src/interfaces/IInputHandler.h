#pragma once

#include "InputKey.h"

class IInputHandler {
public:
    virtual ~IInputHandler() = default;
    
    virtual InputKey getInput() = 0;
    virtual bool isKeyReleased(int vkCode) = 0;
};
