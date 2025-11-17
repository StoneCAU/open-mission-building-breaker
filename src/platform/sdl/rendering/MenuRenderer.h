#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "../../../interfaces/InputKey.h"
#include "../assets/AssetManager.h"

class MenuRenderer {
public:
    MenuRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(int highScore);
    void handleInput(InputKey key);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
    static constexpr int CENTER_X = 400;

    static constexpr int TITLE_Y = 10;
    static constexpr int TITLE_SCALE_NUMERATOR = 5;
    static constexpr int TITLE_SCALE_DENOMINATOR = 10;

    static constexpr int SCORE_BOX_X = 250;
    static constexpr int SCORE_BOX_Y = 300;
    static constexpr int SCORE_BOX_WIDTH = 300;
    static constexpr int SCORE_BOX_HEIGHT = 60;
    static constexpr int SCORE_BOX_BORDER_SIZE = 2;
    static constexpr int SCORE_TITLE_Y_OFFSET = 12;
    static constexpr int SCORE_TEXT_Y_OFFSET = 35;

    static constexpr int START_BUTTON_Y = 450;
    static constexpr int QUIT_BUTTON_Y = 480;
    static constexpr int CREDIT_LINE1_Y = 540;
    static constexpr int CREDIT_LINE2_Y = 565;

    static constexpr SDL_Color FALLBACK_BG_COLOR = {26, 26, 26, 255};
    static constexpr SDL_Color SCORE_BOX_COLOR = {45, 27, 20, 180};
    static constexpr SDL_Color SCORE_BORDER_COLOR = {139, 0, 0, 255};
    static constexpr SDL_Color SCORE_TITLE_COLOR = {218, 165, 32, 255};
    static constexpr SDL_Color SCORE_TEXT_COLOR = {245, 245, 220, 255};
    static constexpr SDL_Color START_BUTTON_COLOR = {139, 0, 0, 255};
    static constexpr SDL_Color QUIT_BUTTON_COLOR = {150, 150, 150, 255};
    static constexpr SDL_Color CREDIT_COLOR1 = {120, 120, 120, 255};
    static constexpr SDL_Color CREDIT_COLOR2 = {110, 110, 110, 255};

    static constexpr const char* TEXT_HIGH_SCORE = "HIGH SCORE";
    static constexpr const char* TEXT_SCORE_SUFFIX = " PTS";
    static constexpr const char* TEXT_START = "PRESS START";
    static constexpr const char* TEXT_QUIT = "Q - QUIT";
    static constexpr const char* TEXT_CREDIT1 = "WOOWA TECH COURSE 8TH - OPEN MISSION";
    static constexpr const char* TEXT_CREDIT2 = "MADE BY HONG SEOKWOO";

    void initializeFrame();
    void renderMenuContent(int highScore);
    void finalizeFrame();

    void renderBackground();
    void renderTitle();
    void renderScoreBox(int highScore);
    void renderButtons();
    void renderScoreBoxBackground();
    void renderScoreBoxBorder();
    void renderScoreContent(int highScore);

    void renderTextCentered(const std::string& text, int x, int y, const std::string& fontName, SDL_Color color);
    SDL_Surface* createTextSurface(const std::string& text, const std::string& fontName, SDL_Color color);
    SDL_Texture* createTextTexture(SDL_Surface* surface);
    void renderTextTexture(SDL_Texture* texture, SDL_Surface* surface, int x, int y);
    void cleanupTextResources(SDL_Surface* surface, SDL_Texture* texture);

    void renderRect(int x, int y, int w, int h, SDL_Color color);
    SDL_Rect calculateTitleRect(int originalW, int originalH);
    SDL_Rect calculateScoreBoxRect();
    SDL_Rect calculateScoreBorderRect();
};
