#pragma once
#include "InputKey.h"

class GameSession;
struct GameOverDisplayData;

class IRenderer {
public:
    virtual ~IRenderer() = default;
    
    virtual void renderMenu(int highScore) = 0;
    virtual void renderPlaying(const GameSession& session) = 0;
    virtual void renderGameOver(const GameOverDisplayData& data) = 0;
    virtual void clearScreen() = 0;
    virtual void clearScreenFull() = 0;
    virtual void flushOutput() = 0;

    virtual void handleMenuInput(InputKey key) = 0;
    virtual void handleGameOverInput(InputKey key) = 0;
};
