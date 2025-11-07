#pragma once
#include <string>
#include "../game/GameSession.h"

class UIRenderer {
public:
    void clearScreen() const;
    void renderMenu(int highScore) const;
    void renderPlaying(const GameSession& session) const;

private:
    // ====== 공통 유틸 ======
    void printBorder() const;
    std::string getGaugeBar(int gauge) const;

    // ====== HUD & GUIDE ======
    void renderHUD(const GameSession& session) const;
    void renderGuide() const;

    // ====== 플레이어 렌더링 ======
    void renderPlayer(const Player& player) const;
    std::string getPlayerMotion(const Player& player) const;
    void renderPlayerLine(int playerX, const std::string& motion) const;
    void renderEmptyLine() const;
};
