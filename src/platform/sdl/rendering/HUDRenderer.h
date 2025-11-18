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
    static constexpr int SCORE_FRAME_WIDTH = 240;
    static constexpr int SCORE_FRAME_HEIGHT = 110;
    static constexpr int SCORE_TEXT_Y_OFFSET = 50;
    static constexpr int SCORE_TEXT_RIGHT_MARGIN = 60;

    static constexpr int COMBO_MEDAL_X = 280;
    static constexpr int COMBO_MEDAL_Y = 15;
    static constexpr int COMBO_MEDAL_SIZE = 100;
    static constexpr int COMBO_TEXT_Y_OFFSET = 42;
    static constexpr int COMBO_TEXT_X_OFFSET = 0;

    static constexpr int GAUGE_X = 50;
    static constexpr int GAUGE_Y_FROM_BOTTOM = 40;
    static constexpr int GAUGE_WIDTH = 160;
    static constexpr int GAUGE_HEIGHT = 20;

    static constexpr int LIVES_START_X = 70;
    static constexpr int LIVES_Y_FROM_BOTTOM = 70;
    static constexpr int LIVES_SPACING = 28;
    static constexpr int LIFE_ICON_SIZE = 24;
    static constexpr int MAX_LIVES = 3;

    static constexpr int SCORE_NUMBER_WIDTH = 12;
    static constexpr int SCORE_NUMBER_HEIGHT = 20;
    static constexpr int SCORE_NUMBER_SPACING = 15;

    static constexpr int COMBO_NUMBER_WIDTH = 12;
    static constexpr int COMBO_NUMBER_HEIGHT = 20;
    static constexpr int COMBO_NUMBER_SPACING = 15;

    static constexpr int SCREEN_HEIGHT = 600;

    static constexpr int GAUGE_MAX_VALUE = 100;
    static constexpr char DIGIT_MIN = '0';
    static constexpr char DIGIT_MAX = '9';

    void renderScore(int score);
    void renderCombo(int combo);
    void renderSpecialGauge(int gauge);
    void renderLives(int lives);
    void renderNumberImages(const std::string& numberStr, int startX, int y, int numberWidth, int numberHeight, int spacing);
    void renderSingleLifeIcon(int index, int lives, int livesY);

    SDL_Rect calculateScoreFrameRect() const;
    SDL_Rect calculateComboMedalRect() const;
    int calculateGaugeY() const;
    int calculateLivesY() const;
    int calculateGaugeFillWidth(int gauge) const;
    bool isValidDigit(char c) const;
    SDL_Texture* getLifeTexture(int index, int lives);
};
