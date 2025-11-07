#pragma once
#include <string>
#include <vector>
#include "../game/GameSession.h"
#include "../game/Player.h"

class UIRenderer {
public:
    // ====== 전체 렌더 흐름 ======
    void clearScreen() const;
    void renderMenu(int highScore) const;
    void renderPlaying(const GameSession& session) const;

private:
    /** ===================== 공통 유틸 ===================== **/
    void printBorder() const;
    bool isInside(int x, int y) const;
    void drawLine(std::vector<std::string>& screen, int x, int y, const std::string& text) const;

    /** ===================== HUD & GUIDE ===================== **/
    std::string getGaugeBar(int gauge) const;
    void renderHUD(const GameSession& session) const;
    void renderGuide() const;

    /** ===================== 본문 렌더링 (빌딩 + 플레이어) ===================== **/
    void renderBody(const GameSession& session) const;
    void composeBuildings(const GameSession& session, std::vector<std::string>& screen) const;
    void composePlayer(const Player& player, std::vector<std::string>& screen) const;
};
