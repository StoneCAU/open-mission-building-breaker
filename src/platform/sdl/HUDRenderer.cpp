#include "HUDRenderer.h"
#include "../../core/game/GameSession.h"
#include "AssetManager.h"

HUDRenderer::HUDRenderer(SDL_Renderer* renderer, AssetManager* assets)
    : renderer(renderer), assets(assets) {}

void HUDRenderer::render(const GameSession& session) {
    renderScore(session.getScore());
    renderCombo(session.getCombo());
    renderSpecialGauge(session.getGauge());
    renderLives(session.getLife());
}

void HUDRenderer::renderScore(int score) {
    SDL_Texture* scoreFrame = assets->getTexture("score_frame");

    const auto renderScoreFrame = [&]() {
        SDL_Rect frameRect = calculateScoreFrameRect();
        SDL_RenderCopy(renderer, scoreFrame, nullptr, &frameRect);

        std::string scoreStr = std::to_string(score);
        int scoreWidth = scoreStr.length() * NUMBER_WIDTH;
        int scoreX = (SCORE_FRAME_X + SCORE_FRAME_WIDTH) - scoreWidth - SCORE_TEXT_RIGHT_MARGIN;
        int scoreY = SCORE_FRAME_Y + SCORE_TEXT_Y_OFFSET;
        renderNumberImages(scoreStr, scoreX, scoreY);
    };

    scoreFrame && (renderScoreFrame(), true);
}

void HUDRenderer::renderCombo(int combo) {
    SDL_Texture* comboMedal = assets->getTexture("combo_medal");

    const auto renderComboMedal = [&]() {
        SDL_Rect medalRect = calculateComboMedalRect();
        SDL_RenderCopy(renderer, comboMedal, nullptr, &medalRect);

        std::string comboStr = std::to_string(combo);
        int comboWidth = comboStr.length() * COMBO_NUMBER_WIDTH;
        int medalCenterX = COMBO_MEDAL_X + (COMBO_MEDAL_SIZE - comboWidth) / 2;
        int comboY = COMBO_MEDAL_Y + SCORE_TEXT_Y_OFFSET;
        renderNumberImages(comboStr, medalCenterX, comboY);
    };

    comboMedal && (renderComboMedal(), true);
}

void HUDRenderer::renderSpecialGauge(int gauge) {
    SDL_Texture* emptyGauge = assets->getTexture("gauge_empty");
    SDL_Texture* fillGauge = assets->getTexture("gauge_fill");

    int gaugeY = calculateGaugeY();

    const auto renderEmptyGauge = [&]() {
        SDL_Rect emptyRect{GAUGE_X, gaugeY, GAUGE_WIDTH, GAUGE_HEIGHT};
        SDL_RenderCopy(renderer, emptyGauge, nullptr, &emptyRect);
    };

    const auto renderFillGauge = [&]() {
        int fillWidth = (gauge * GAUGE_WIDTH) / 100;
        SDL_Rect srcRect{0, 0, fillWidth, GAUGE_HEIGHT};
        SDL_Rect dstRect{GAUGE_X, gaugeY, fillWidth, GAUGE_HEIGHT};
        SDL_RenderCopy(renderer, fillGauge, &srcRect, &dstRect);
    };

    emptyGauge && (renderEmptyGauge(), true);
    fillGauge && gauge > 0 && (renderFillGauge(), true);
}

void HUDRenderer::renderLives(int lives) {
    SDL_Texture* activeLife = assets->getTexture("life_active");
    SDL_Texture* inactiveLife = assets->getTexture("life_inactive");

    int livesY = calculateLivesY();

    const auto renderAllLives = [&]() {
        for (int i = 0; i < MAX_LIVES; ++i) {
            const auto selectTexture = [&]() { return (i < lives) ? activeLife : inactiveLife; };
            SDL_Texture* lifeTexture = selectTexture();
            SDL_Rect lifeRect{LIVES_START_X + i * LIVES_SPACING, livesY, LIFE_ICON_SIZE, LIFE_ICON_SIZE};
            SDL_RenderCopy(renderer, lifeTexture, nullptr, &lifeRect);
        }
    };

    activeLife && inactiveLife && (renderAllLives(), true);
}

void HUDRenderer::renderNumberImages(const std::string& numberStr, int startX, int y) {
    for (int i = 0; i < numberStr.length(); ++i) {
        const auto isDigit = [&]() { return numberStr[i] >= '0' && numberStr[i] <= '9'; };

        const auto renderDigit = [&]() {
            int digit = numberStr[i] - '0';
            SDL_Texture* numberTexture = assets->getTexture("number_" + std::to_string(digit));

            const auto renderNumberTexture = [&]() {
                SDL_Rect numberRect{startX + i * NUMBER_WIDTH, y, NUMBER_IMAGE_WIDTH, NUMBER_HEIGHT};
                SDL_RenderCopy(renderer, numberTexture, nullptr, &numberRect);
            };

            numberTexture && (renderNumberTexture(), true);
        };

        isDigit() && (renderDigit(), true);
    }
}

SDL_Rect HUDRenderer::calculateScoreFrameRect() const {
    return {SCORE_FRAME_X, SCORE_FRAME_Y, SCORE_FRAME_WIDTH, SCORE_FRAME_HEIGHT};
}

SDL_Rect HUDRenderer::calculateComboMedalRect() const {
    return {COMBO_MEDAL_X, COMBO_MEDAL_Y, COMBO_MEDAL_SIZE, COMBO_MEDAL_SIZE};
}

int HUDRenderer::calculateGaugeY() const {
    return SCREEN_HEIGHT - GAUGE_Y_FROM_BOTTOM;
}

int HUDRenderer::calculateLivesY() const {
    return SCREEN_HEIGHT - LIVES_Y_FROM_BOTTOM;
}
