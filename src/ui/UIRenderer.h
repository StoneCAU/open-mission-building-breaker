#pragma once
#include <string>

#include "../game/GameSession.h"

class UIRenderer {
public:
    void renderMenu(int highScore) const;
    void renderPlaying(const GameSession& session) const;

private:
    void printBorder() const;
    std::string getGaugeBar(int gauge) const;
};
