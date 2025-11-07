#pragma once
#include <string>
#include <windows.h>

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

    // ====== 플레이어 & 빌딩 통합 렌더링 ======
    void composeBuildings(const GameSession& session, std::vector<std::string>& screen) const;
    void composePlayer(const Player& player, std::vector<std::string>& screen) const;
};
