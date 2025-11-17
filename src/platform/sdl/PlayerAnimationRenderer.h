#pragma once
#include <SDL2/SDL.h>
#include <string>

class Player;
class AssetManager;

class PlayerAnimationRenderer {
public:
    PlayerAnimationRenderer(AssetManager* assets);

    void render(SDL_Renderer* renderer, const Player& player, int screenX, int screenY);

private:
    AssetManager* assets;

    struct AnimationState {
        bool lastMovingLeft = false;
        bool lastMovingRight = false;
        int idleFrames = 0;
        bool facingLeft = false;
        int frameCounter = 0;
    };

    mutable AnimationState state;

    bool isActuallyMoving(const Player& player) const;
    std::string determineSpriteName(const Player& player) const;
    bool shouldFlipSprite(const Player& player, bool actuallyMoving) const;
    void updateAnimationState(const Player& player, bool actuallyMoving) const;
};
