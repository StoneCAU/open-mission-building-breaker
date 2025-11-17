#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../../../interfaces/IInputHandler.h"

class AssetManager;
struct GameOverDisplayData;

class GameOverRenderer {
public:
    GameOverRenderer(SDL_Renderer* renderer, AssetManager* assets);

    void render(const GameOverDisplayData& data);
    void handleInput(InputKey key);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;

    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 600;
    static constexpr int TITLE_Y = 120;
    static constexpr int FINAL_SCORE_Y = 220;
    static constexpr int MAX_COMBO_Y = 250;
    static constexpr int PLAY_TIME_Y = 280;
    static constexpr int NEW_RECORD_Y = 320;
    static constexpr int RESTART_BUTTON_Y = 450;
    static constexpr int QUIT_BUTTON_Y = 480;
    static constexpr int CENTER_X = 400;

    static constexpr SDL_Color FALLBACK_BG_COLOR = {25, 25, 40, 255};
    static constexpr SDL_Color TITLE_COLOR = {255, 100, 100, 255};
    static constexpr SDL_Color SCORE_COLOR = {255, 255, 255, 255};
    static constexpr SDL_Color COMBO_COLOR = {255, 255, 100, 255};
    static constexpr SDL_Color TIME_COLOR = {200, 200, 200, 255};
    static constexpr SDL_Color RECORD_COLOR = {255, 255, 100, 255};
    static constexpr SDL_Color RESTART_COLOR = {100, 255, 100, 255};
    static constexpr SDL_Color QUIT_COLOR = {255, 100, 100, 255};

    static constexpr const char* TEXT_GAME_OVER = "게임 오버";
    static constexpr const char* TEXT_FINAL_SCORE = "최종 점수: ";
    static constexpr const char* TEXT_SCORE_UNIT = "점";
    static constexpr const char* TEXT_MAX_COMBO = "최고 콤보: x";
    static constexpr const char* TEXT_PLAY_TIME = "플레이 시간: ";
    static constexpr const char* TEXT_NEW_RECORD = "새로운 최고 기록!";
    static constexpr const char* TEXT_RESTART = "[R] 재시작";
    static constexpr const char* TEXT_QUIT = "[Q] 종료";
    static constexpr const char* TEXT_MINUTE = "분 ";
    static constexpr const char* TEXT_SECOND = "초";

    void renderTextCentered(const std::string& text, int x, int y, SDL_Color color);
    void renderBackground();
    void renderGameOverContent(const GameOverDisplayData& data);
    void renderScoreSection(const GameOverDisplayData& data);
    void renderControlButtons();

    void createTextTexture(const std::string& text, SDL_Color color, TTF_Font* font, int x, int y);
    void renderTextTexture(SDL_Texture* texture, int x, int y, int width, int height);

    std::string buildGameTimeDisplay(int totalSeconds);
    std::string formatScoreText(int score);
    std::string formatComboText(int combo);
    std::string formatTimeText(int seconds);
};
