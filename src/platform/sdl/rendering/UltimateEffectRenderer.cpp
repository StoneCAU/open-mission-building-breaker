#include "UltimateEffectRenderer.h"
#include "../../../core/game/GameSession.h"
#include "../assets/AssetConfig.h"
#include "../assets/AssetManager.h"
#include "../assets/SoundManager.h"

UltimateEffectRenderer::UltimateEffectRenderer(AssetManager* a)
    : assets(a) {}

void UltimateEffectRenderer::update(const GameSession& session) {
    const auto shouldTriggerEffect = [&]() {
        return session.justUsedUltimate() && effectFrames == 0;
    };

    shouldTriggerEffect() && (triggerUltimateEffect(), true);
}

void UltimateEffectRenderer::triggerUltimateEffect() {
    SoundManager::playImmediate(AssetConfig::SOUND_SPECIAL);
    effectFrames = EFFECT_DURATION_FRAMES;
}

void UltimateEffectRenderer::render(SDL_Renderer* renderer, int centerX, int centerY) {
    (effectFrames > 0) && (renderEffectIfActive(renderer, centerX, centerY), true);
}

void UltimateEffectRenderer::renderEffectIfActive(SDL_Renderer* renderer, int centerX, int centerY) {
    SDL_Texture* hurricane = assets->getTexture(AssetConfig::TEXTURE_HURRICANE_EFFECT);

    const auto processHurricaneTexture = [&]() {
        updateRotation();

        int alpha = calculateAlpha();
        SDL_SetTextureAlphaMod(hurricane, alpha);

        int size = calculateSize();
        SDL_Rect destRect = calculateDestRect(centerX, centerY, size);

        SDL_RenderCopyEx(renderer, hurricane, nullptr, &destRect,
                         rotation, nullptr, SDL_FLIP_NONE);

        decrementEffectFrames();
    };

    hurricane && (processHurricaneTexture(), true);
}

void UltimateEffectRenderer::updateRotation() {
    rotation += ROTATION_SPEED;
}

int UltimateEffectRenderer::calculateAlpha() const {
    return (effectFrames * ALPHA_MAX) / EFFECT_DURATION_FRAMES;
}

float UltimateEffectRenderer::calculateScale() const {
    float progress = (EFFECT_DURATION_FRAMES - effectFrames) / static_cast<float>(EFFECT_DURATION_FRAMES);
    return BASE_SCALE + (progress * MAX_SCALE_MULTIPLIER);
}

int UltimateEffectRenderer::calculateSize() const {
    float scale = calculateScale();
    return static_cast<int>(BASE_SIZE * scale);
}

SDL_Rect UltimateEffectRenderer::calculateDestRect(int centerX, int centerY, int size) const {
    return {
        centerX - size / 2,
        centerY - size / 2,
        size,
        size
    };
}

void UltimateEffectRenderer::decrementEffectFrames() {
    --effectFrames;
}
