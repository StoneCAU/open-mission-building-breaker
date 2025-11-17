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
        SDL_Rect frameRect{20, 15, 160, 60};
        SDL_RenderCopy(renderer, scoreFrame, nullptr, &frameRect);

        std::string scoreStr = std::to_string(score);
        int scoreWidth = scoreStr.length() * 12;
        int scoreX = (20 + 160) - scoreWidth - 40;
        renderNumberImages(scoreStr, scoreX, 35);
    };

    scoreFrame && (renderScoreFrame(), true);
}

void HUDRenderer::renderCombo(int combo) {
    SDL_Texture* comboMedal = assets->getTexture("combo_medal");

    const auto renderComboMedal = [&]() {
        SDL_Rect medalRect{200, 15, 60, 60};
        SDL_RenderCopy(renderer, comboMedal, nullptr, &medalRect);

        std::string comboStr = std::to_string(combo);
        int comboWidth = comboStr.length() * 14;
        int medalCenterX = 200 + (60 - comboWidth) / 2;
        renderNumberImages(comboStr, medalCenterX, 35);
    };

    comboMedal && (renderComboMedal(), true);
}

void HUDRenderer::renderSpecialGauge(int gauge) {
    SDL_Texture* emptyGauge = assets->getTexture("gauge_empty");
    SDL_Texture* fillGauge = assets->getTexture("gauge_fill");

    int gaugeX = 50;
    int gaugeY = 600 - 40;
    int gaugeWidth = 160;
    int gaugeHeight = 20;

    const auto renderEmptyGauge = [&]() {
        SDL_Rect emptyRect{gaugeX, gaugeY, gaugeWidth, gaugeHeight};
        SDL_RenderCopy(renderer, emptyGauge, nullptr, &emptyRect);
    };

    const auto renderFillGauge = [&]() {
        int fillWidth = (gauge * gaugeWidth) / 100;
        SDL_Rect srcRect{0, 0, fillWidth, gaugeHeight};
        SDL_Rect dstRect{gaugeX, gaugeY, fillWidth, gaugeHeight};
        SDL_RenderCopy(renderer, fillGauge, &srcRect, &dstRect);
    };

    emptyGauge && (renderEmptyGauge(), true);
    fillGauge && gauge > 0 && (renderFillGauge(), true);
}

void HUDRenderer::renderLives(int lives) {
    SDL_Texture* activeLife = assets->getTexture("life_active");
    SDL_Texture* inactiveLife = assets->getTexture("life_inactive");

    int startX = 70;
    int y = 600 - 70;
    int maxLives = 3;

    const auto renderAllLives = [&]() {
        for (int i = 0; i < maxLives; ++i) {
            const auto selectTexture = [&]() { return (i < lives) ? activeLife : inactiveLife; };
            SDL_Texture* lifeTexture = selectTexture();
            SDL_Rect lifeRect{startX + i * 28, y, 24, 24};
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
                SDL_Rect numberRect{startX + i * 12, y, 16, 20};
                SDL_RenderCopy(renderer, numberTexture, nullptr, &numberRect);
            };

            numberTexture && (renderNumberTexture(), true);
        };

        isDigit() && (renderDigit(), true);
    }
}
