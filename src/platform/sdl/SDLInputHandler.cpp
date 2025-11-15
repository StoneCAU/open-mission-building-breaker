#include "SDLInputHandler.h"
#include <windows.h>

SDLInputHandler::SDLInputHandler()
    : quitRequested(false) {
    initializeKeyMap();
    keyboardState = SDL_GetKeyboardState(nullptr);
}

void SDLInputHandler::initializeKeyMap() {
    keyMap[SDL_SCANCODE_LEFT] = InputKey::LEFT;
    keyMap[SDL_SCANCODE_RIGHT] = InputKey::RIGHT;
    keyMap[SDL_SCANCODE_UP] = InputKey::JUMP;
    keyMap[SDL_SCANCODE_DOWN] = InputKey::DEFEND;
    keyMap[SDL_SCANCODE_Z] = InputKey::ATTACK;
    keyMap[SDL_SCANCODE_X] = InputKey::ULTIMATE;
    keyMap[SDL_SCANCODE_RETURN] = InputKey::ENTER;
    keyMap[SDL_SCANCODE_Q] = InputKey::QUIT;
    keyMap[SDL_SCANCODE_R] = InputKey::RESTART;
}

bool SDLInputHandler::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quitRequested = true;
            return false;
        }
    }
    
    keyboardState = SDL_GetKeyboardState(nullptr);
    return true;
}

bool SDLInputHandler::shouldQuit() const {
    return quitRequested;
}

InputKey SDLInputHandler::getInput() {
    InputKey key = checkCombinedInput();
    if (key != InputKey::NONE) return key;
    
    key = checkDirectionalInput();
    if (key != InputKey::NONE) return key;
    
    return checkGeneralInput();
}

InputKey SDLInputHandler::checkCombinedInput() {
    bool left = keyboardState[SDL_SCANCODE_LEFT];
    bool right = keyboardState[SDL_SCANCODE_RIGHT];
    bool up = keyboardState[SDL_SCANCODE_UP];
    
    if (left && up) return InputKey::MOVE_LEFT_JUMP;
    if (right && up) return InputKey::MOVE_RIGHT_JUMP;
    
    return InputKey::NONE;
}

InputKey SDLInputHandler::checkDirectionalInput() {
    if (keyboardState[SDL_SCANCODE_UP]) return InputKey::JUMP;
    if (keyboardState[SDL_SCANCODE_DOWN]) return InputKey::DEFEND;
    if (keyboardState[SDL_SCANCODE_LEFT]) return InputKey::LEFT;
    if (keyboardState[SDL_SCANCODE_RIGHT]) return InputKey::RIGHT;
    
    return InputKey::NONE;
}

InputKey SDLInputHandler::checkGeneralInput() {
    if (keyboardState[SDL_SCANCODE_Z]) return InputKey::ATTACK;
    if (keyboardState[SDL_SCANCODE_X]) return InputKey::ULTIMATE;
    if (keyboardState[SDL_SCANCODE_Q]) return InputKey::QUIT;
    if (keyboardState[SDL_SCANCODE_RETURN]) return InputKey::ENTER;
    if (keyboardState[SDL_SCANCODE_R]) return InputKey::RESTART;
    
    return InputKey::NONE;
}

bool SDLInputHandler::isKeyReleased(int vkCode) {
    SDL_Scancode scancode;
    
    if (vkCode == VK_UP) scancode = SDL_SCANCODE_UP;
    else if (vkCode == VK_DOWN) scancode = SDL_SCANCODE_DOWN;
    else return true;
    
    return !keyboardState[scancode];
}
