#include "PlayerAnimationRenderer.h"
#include "../../core/player/Player.h"
#include "AssetManager.h"
#include <functional>
#include <vector>
#include <algorithm>

PlayerAnimationRenderer::PlayerAnimationRenderer(AssetManager* assets) : assets(assets) {}

void PlayerAnimationRenderer::render(SDL_Renderer* renderer, const Player& player, int screenX, int screenY) {
    bool actuallyMoving = isActuallyMoving(player);
    updateAnimationState(player, actuallyMoving);

    std::string spriteName = determineSpriteName(player);
    bool shouldFlip = shouldFlipSprite(player, actuallyMoving);

    SDL_Texture* sprite = assets->getTexture(spriteName);
    SDL_Rect destRect = {screenX, screenY, 32, 32};
    SDL_RendererFlip flip = shouldFlip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    sprite && (SDL_RenderCopyEx(renderer, sprite, nullptr, &destRect, 0.0, nullptr, flip), true);
}

bool PlayerAnimationRenderer::isActuallyMoving(const Player& player) const {
    bool currentLeft = player.isMovingLeft();
    bool currentRight = player.isMovingRight();

    const auto stateChanged = [&]() {
        return currentLeft != state.lastMovingLeft || currentRight != state.lastMovingRight;
    };

    state.idleFrames = stateChanged() ? 0 : state.idleFrames + 1;

    return (currentLeft || currentRight) && (state.idleFrames < 5);
}

std::string PlayerAnimationRenderer::determineSpriteName(const Player& player) const {
    const std::vector<std::pair<std::function<bool()>, std::string>> stateCheckers = {
        {[&]() { return player.isDamaged(); }, "player_hit"},
        {[&]() { return player.getAction() == PlayerActionType::DEFEND; },
         [&]() {
             int frame = (state.frameCounter / 30) % 2 + 1;
             return "player_defence_" + std::to_string(frame);
         }()},
        {[&]() { return isActuallyMoving(player); },
         [&]() {
             int frame = (state.frameCounter / 15) % 3 + 1;
             return "player_move_" + std::to_string(frame);
         }()},
        {[&]() { return player.getAction() == PlayerActionType::ATTACK; }, "player_attack"}
    };

    std::string spriteName = "player_idle";
    std::for_each(stateCheckers.begin(), stateCheckers.end(),
        [&](const auto& checker) {
            checker.first() && (spriteName = checker.second, true);
        });

    return spriteName;
}

bool PlayerAnimationRenderer::shouldFlipSprite(const Player& player, bool actuallyMoving) const {
    return actuallyMoving ? player.isMovingLeft() : state.facingLeft;
}

void PlayerAnimationRenderer::updateAnimationState(const Player& player, bool actuallyMoving) const {
    bool currentLeft = player.isMovingLeft();
    bool currentRight = player.isMovingRight();

    currentLeft && (state.facingLeft = true);
    currentRight && (state.facingLeft = false);

    actuallyMoving ? ++state.frameCounter : (state.frameCounter = 0);

    state.lastMovingLeft = currentLeft;
    state.lastMovingRight = currentRight;
}
