#include "SDLInputHandler.h"
#include <windows.h>

#include "../../../interfaces/InputKey.h"

SDLInputHandler::SDLInputHandler()
    : quitRequested(false) {
    initializeKeyMap();
    keyboardState = SDL_GetKeyboardState(nullptr);
}

void SDLInputHandler::initializeKeyMap() {
    keyMap[KEY_LEFT] = InputKey::LEFT;
    keyMap[KEY_RIGHT] = InputKey::RIGHT;
    keyMap[KEY_UP] = InputKey::JUMP;
    keyMap[KEY_DOWN] = InputKey::DEFEND;
    keyMap[KEY_ATTACK] = InputKey::ATTACK;
    keyMap[KEY_ULTIMATE] = InputKey::ULTIMATE;
    keyMap[KEY_ENTER] = InputKey::ENTER;
    keyMap[KEY_QUIT] = InputKey::QUIT;
    keyMap[KEY_RESTART] = InputKey::RESTART;
}

bool SDLInputHandler::pollEvents() {
    SDL_Event event;

    const auto processEvent = [&]() {
        (event.type == SDL_QUIT) && (quitRequested = true, true);
        return event.type != SDL_QUIT;
    };

    while (SDL_PollEvent(&event)) {
        !processEvent() && (false, true);
    }

    keyboardState = SDL_GetKeyboardState(nullptr);
    return !quitRequested;
}

bool SDLInputHandler::shouldQuit() const {
    return quitRequested;
}

InputKey SDLInputHandler::getInput() {
    InputKey key = checkCombinedInput();
    key != InputKey::NONE && (key, true);

    key == InputKey::NONE && (key = checkDirectionalInput(), true);
    key == InputKey::NONE && (key = checkGeneralInput(), true);

    return key;
}

InputKey SDLInputHandler::checkCombinedInput() {
    InputKey result = InputKey::NONE;

    isCombinationPressed(KEY_LEFT, KEY_UP) && (result = InputKey::MOVE_LEFT_JUMP, true);
    result == InputKey::NONE && isCombinationPressed(KEY_RIGHT, KEY_UP) && (result = InputKey::MOVE_RIGHT_JUMP, true);

    return result;
}

InputKey SDLInputHandler::checkDirectionalInput() {
    InputKey result = InputKey::NONE;

    isKeyPressed(KEY_UP) && (result = InputKey::JUMP, true);
    result == InputKey::NONE && isKeyPressed(KEY_DOWN) && (result = InputKey::DEFEND, true);
    result == InputKey::NONE && isKeyPressed(KEY_LEFT) && (result = InputKey::LEFT, true);
    result == InputKey::NONE && isKeyPressed(KEY_RIGHT) && (result = InputKey::RIGHT, true);

    return result;
}

InputKey SDLInputHandler::checkGeneralInput() {
    InputKey result = InputKey::NONE;

    isKeyPressed(KEY_ATTACK) && (result = InputKey::ATTACK, true);
    result == InputKey::NONE && isKeyPressed(KEY_ULTIMATE) && (result = InputKey::ULTIMATE, true);
    result == InputKey::NONE && isKeyPressed(KEY_QUIT) && (result = InputKey::QUIT, true);
    result == InputKey::NONE && isKeyPressed(KEY_ENTER) && (result = InputKey::ENTER, true);
    result == InputKey::NONE && isKeyPressed(KEY_RESTART) && (result = InputKey::RESTART, true);

    return result;
}

bool SDLInputHandler::isKeyPressed(SDL_Scancode scancode) const {
    return keyboardState[scancode];
}

bool SDLInputHandler::isCombinationPressed(SDL_Scancode key1, SDL_Scancode key2) const {
    return isKeyPressed(key1) && isKeyPressed(key2);
}

bool SDLInputHandler::isKeyReleased(int vkCode) {
    SDL_Scancode scancode = mapVirtualKeyToScancode(vkCode);

    return scancode != SDL_SCANCODE_UNKNOWN && !isKeyPressed(scancode);
}

SDL_Scancode SDLInputHandler::mapVirtualKeyToScancode(int vkCode) const {
    SDL_Scancode result = SDL_SCANCODE_UNKNOWN;

    (vkCode == VK_UP) && (result = KEY_UP, true);
    (vkCode == VK_DOWN) && (result = KEY_DOWN, true);

    return result;
}
