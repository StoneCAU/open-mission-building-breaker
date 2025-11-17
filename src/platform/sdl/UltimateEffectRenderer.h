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
    int previousGauge = 0;
    int effectFrames = 0;
    float rotation = 0.0f;
    bool gameStarted = false;
    
    void checkTrigger(int currentGauge);
};
