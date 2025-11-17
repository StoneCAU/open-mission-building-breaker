#include "UltimateEffectRenderer.h"

#include <iostream>

#include "AssetManager.h"
#include "SoundManager.h"
#include "../../core/game/GameSession.h"

UltimateEffectRenderer::UltimateEffectRenderer(AssetManager* a) 
    : assets(a) {}

void UltimateEffectRenderer::update(const GameSession& session) {

    if (session.justUsedUltimate() && effectFrames == 0) {
        SoundManager::playImmediate("special");
        effectFrames = 30;
    }
}

void UltimateEffectRenderer::render(SDL_Renderer* renderer, int centerX, int centerY) {
    if (effectFrames <= 0) return;

    SDL_Texture* hurricane = assets->getTexture("hurricane_effect");
    if (!hurricane) return;

    rotation += 15.0f;

    int alpha = (effectFrames * 255) / 30;
    SDL_SetTextureAlphaMod(hurricane, alpha);

    float progress = (30.0f - effectFrames) / 30.0f;
    float scale = 1.0f + (progress * 2.0f);
    int size = static_cast<int>(100 * scale);

    SDL_Rect destRect = {
        centerX - size / 2,
        centerY - size / 2,
        size,
        size
    };

    SDL_RenderCopyEx(renderer, hurricane, nullptr, &destRect,
                     rotation, nullptr, SDL_FLIP_NONE);

    effectFrames--;
}
