#include "PlayerAnimationRenderer.h"
#include "../../core/player/Player.h"
#include "assets/AssetManager.h"

PlayerAnimationRenderer::PlayerAnimationRenderer(AssetManager* assets) : assets(assets) {}

void PlayerAnimationRenderer::render(SDL_Renderer* renderer, const Player& player, int screenX, int screenY) {
    bool actuallyMoving = isActuallyMoving(player);
    updateAnimationState(player, actuallyMoving);

    std::string spriteName = determineSpriteName(player);
    bool shouldFlip = shouldFlipSprite(player, actuallyMoving);

    SDL_Texture* sprite = assets->getTexture(spriteName);
    SDL_Rect destRect = {screenX, screenY, SPRITE_SIZE, SPRITE_SIZE};

    SDL_RendererFlip flip = SDL_FLIP_NONE;
    shouldFlip && (flip = SDL_FLIP_HORIZONTAL, true);

    sprite && (SDL_RenderCopyEx(renderer, sprite, nullptr, &destRect, 0.0, nullptr, flip), true);
}

bool PlayerAnimationRenderer::isActuallyMoving(const Player& player) const {
    bool currentLeft = player.isMovingLeft();
    bool currentRight = player.isMovingRight();

    updateIdleFrames(currentLeft, currentRight);

    return (currentLeft || currentRight) && (state.idleFrames < IDLE_FRAME_THRESHOLD);
}

void PlayerAnimationRenderer::updateIdleFrames(bool currentLeft, bool currentRight) const {
    const auto stateChanged = [&]() {
        return currentLeft != state.lastMovingLeft || currentRight != state.lastMovingRight;
    };

    stateChanged() && (state.idleFrames = 0, true);
    !stateChanged() && (++state.idleFrames, true);
}

std::string PlayerAnimationRenderer::determineSpriteName(const Player& player) const {
    std::string result;

    result = checkDamagedSprite(player);
    !result.empty() && (result, true);

    result.empty() && (result = checkDefenseSprite(player), true);
    result.empty() && (result = checkMovementSprite(player), true);
    result.empty() && (result = checkAttackSprite(player), true);
    result.empty() && (result = SPRITE_IDLE, true);

    return result;
}

std::string PlayerAnimationRenderer::checkDamagedSprite(const Player& player) const {
    std::string result = "";

    player.isDamaged() && (result = SPRITE_HIT, true);

    return result;
}

std::string PlayerAnimationRenderer::checkDefenseSprite(const Player& player) const {
    std::string result = "";

    (player.getAction() == PlayerActionType::DEFEND) && (result = generateDefenseSpriteName(), true);

    return result;
}

std::string PlayerAnimationRenderer::checkMovementSprite(const Player& player) const {
    std::string result = "";

    isActuallyMoving(player) && (result = generateMovementSpriteName(), true);

    return result;
}

std::string PlayerAnimationRenderer::checkAttackSprite(const Player& player) const {
    std::string result = "";

    (player.getAction() == PlayerActionType::ATTACK) && (result = SPRITE_ATTACK, true);

    return result;
}

std::string PlayerAnimationRenderer::generateDefenseSpriteName() const {
    int frame = (state.frameCounter / DEFENSE_ANIMATION_CYCLE) % DEFENSE_FRAME_COUNT + 1;
    return SPRITE_DEFENSE_PREFIX + std::to_string(frame);
}

std::string PlayerAnimationRenderer::generateMovementSpriteName() const {
    int frame = (state.frameCounter / MOVE_ANIMATION_CYCLE) % MOVE_FRAME_COUNT + 1;
    return SPRITE_MOVE_PREFIX + std::to_string(frame);
}

bool PlayerAnimationRenderer::shouldFlipSprite(const Player& player, bool actuallyMoving) const {
    bool result = state.facingLeft;

    actuallyMoving && (result = player.isMovingLeft(), true);

    return result;
}

void PlayerAnimationRenderer::updateAnimationState(const Player& player, bool actuallyMoving) const {
    bool currentLeft = player.isMovingLeft();
    bool currentRight = player.isMovingRight();

    updateMovementState(currentLeft, currentRight);
    updateFrameCounter(actuallyMoving);

    state.lastMovingLeft = currentLeft;
    state.lastMovingRight = currentRight;
}

void PlayerAnimationRenderer::updateMovementState(bool currentLeft, bool currentRight) const {
    currentLeft && (state.facingLeft = true, true);
    currentRight && (state.facingLeft = false, true);
}

void PlayerAnimationRenderer::updateFrameCounter(bool actuallyMoving) const {
    actuallyMoving && (++state.frameCounter, true);
    !actuallyMoving && (state.frameCounter = 0, true);
}
