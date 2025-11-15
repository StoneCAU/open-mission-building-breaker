#pragma once

#include "../../interfaces/IInputHandler.h"
#include <SDL2/SDL.h>
#include <unordered_map>

class SDLInputHandler : public IInputHandler {
public:
    SDLInputHandler();
    
    InputKey getInput() override;
    bool isKeyReleased(int vkCode) override;
    
    bool pollEvents();
    bool shouldQuit() const;

private:
    std::unordered_map<SDL_Scancode, InputKey> keyMap;
    const Uint8* keyboardState;
    bool quitRequested;
    
    void initializeKeyMap();
    InputKey checkCombinedInput();
    InputKey checkDirectionalInput();
    InputKey checkGeneralInput();
};
