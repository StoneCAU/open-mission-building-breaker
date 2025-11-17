#pragma once
#include "../../../interfaces/IInputHandler.h"
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

    static constexpr SDL_Scancode KEY_LEFT = SDL_SCANCODE_LEFT;
    static constexpr SDL_Scancode KEY_RIGHT = SDL_SCANCODE_RIGHT;
    static constexpr SDL_Scancode KEY_UP = SDL_SCANCODE_UP;
    static constexpr SDL_Scancode KEY_DOWN = SDL_SCANCODE_DOWN;
    static constexpr SDL_Scancode KEY_ATTACK = SDL_SCANCODE_Z;
    static constexpr SDL_Scancode KEY_ULTIMATE = SDL_SCANCODE_X;
    static constexpr SDL_Scancode KEY_ENTER = SDL_SCANCODE_RETURN;
    static constexpr SDL_Scancode KEY_QUIT = SDL_SCANCODE_Q;
    static constexpr SDL_Scancode KEY_RESTART = SDL_SCANCODE_R;

    void initializeKeyMap();
    InputKey checkCombinedInput();
    InputKey checkDirectionalInput();
    InputKey checkGeneralInput();

    InputKey checkCombinationKeys();
    InputKey checkMovementKeys();
    InputKey checkActionKeys();
    InputKey checkSystemKeys();

    bool isKeyPressed(SDL_Scancode scancode) const;
    bool isCombinationPressed(SDL_Scancode key1, SDL_Scancode key2) const;
    SDL_Scancode mapVirtualKeyToScancode(int vkCode) const;
};
