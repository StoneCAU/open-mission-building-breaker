#pragma once
#include <SDL2/SDL.h>
#include <string>

class GameSession;
class AssetManager;

class HUDRenderer {
public:
    HUDRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(const GameSession& session);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    static constexpr int SCORE_FRAME_X = 20;
    static constexpr int SCORE_FRAME_Y = 15;
    static constexpr int SCORE_FRAME_WIDTH = 160;
    static constexpr int SCORE_FRAME_HEIGHT = 60;
    static constexpr int SCORE_TEXT_Y_OFFSET = 20;
    static constexpr int SCORE_TEXT_RIGHT_MARGIN = 40;

    static constexpr int COMBO_MEDAL_X = 200;
    static constexpr int COMBO_MEDAL_Y = 15;
    static constexpr int COMBO_MEDAL_SIZE = 60;

    static constexpr int GAUGE_X = 50;
    static constexpr int GAUGE_Y_FROM_BOTTOM = 40;
    static constexpr int GAUGE_WIDTH = 160;
    static constexpr int GAUGE_HEIGHT = 20;

    static constexpr int LIVES_START_X = 70;
    static constexpr int LIVES_Y_FROM_BOTTOM = 70;
    static constexpr int LIVES_SPACING = 28;
    static constexpr int LIFE_ICON_SIZE = 24;
    static constexpr int MAX_LIVES = 3;

    static constexpr int NUMBER_WIDTH = 12;
    static constexpr int NUMBER_HEIGHT = 20;
    static constexpr int NUMBER_IMAGE_WIDTH = 16;
    static constexpr int COMBO_NUMBER_WIDTH = 14;

    static constexpr int SCREEN_HEIGHT = 600;

    void renderScore(int score);
    void renderCombo(int combo);
    void renderSpecialGauge(int gauge);
    void renderLives(int lives);
    void renderNumberImages(const std::string& numberStr, int startX, int y);

    SDL_Rect calculateScoreFrameRect() const;
    SDL_Rect calculateComboMedalRect() const;
    int calculateGaugeY() const;
    int calculateLivesY() const;
};
