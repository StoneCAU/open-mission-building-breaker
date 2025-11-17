#pragma once
#include <SDL2/SDL.h>

class GameSession;
class AssetManager;

class UltimateEffectRenderer {
public:
    UltimateEffectRenderer(AssetManager* assets);
    
    void update(const GameSession& session);
    void render(SDL_Renderer* renderer, int centerX, int centerY);
    
private:
    AssetManager* assets;
    int effectFrames = 0;
    float rotation = 0.0f;

    static constexpr int EFFECT_DURATION_FRAMES = 30;
    static constexpr float ROTATION_SPEED = 15.0f;
    static constexpr int ALPHA_MAX = 255;
    static constexpr float BASE_SCALE = 1.0f;
    static constexpr float MAX_SCALE_MULTIPLIER = 2.0f;
    static constexpr int BASE_SIZE = 100;

    void triggerUltimateEffect();
    void renderEffectIfActive(SDL_Renderer* renderer, int centerX, int centerY);
    void updateRotation();
    int calculateAlpha() const;
    float calculateScale() const;
    int calculateSize() const;
    SDL_Rect calculateDestRect(int centerX, int centerY, int size) const;
    void decrementEffectFrames();
};
