#pragma once
#include <SDL2/SDL.h>
#include <string>

class Player;
class AssetManager;
enum class PlayerActionType;

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

    static constexpr int SPRITE_SIZE = 32;
    static constexpr int IDLE_FRAME_THRESHOLD = 5;
    static constexpr int DEFENSE_ANIMATION_CYCLE = 30;
    static constexpr int MOVE_ANIMATION_CYCLE = 15;
    static constexpr int DEFENSE_FRAME_COUNT = 2;
    static constexpr int MOVE_FRAME_COUNT = 3;

    static constexpr const char* SPRITE_IDLE = "player_idle";
    static constexpr const char* SPRITE_HIT = "player_hit";
    static constexpr const char* SPRITE_ATTACK = "player_attack";
    static constexpr const char* SPRITE_DEFENSE_PREFIX = "player_defence_";
    static constexpr const char* SPRITE_MOVE_PREFIX = "player_move_";

    bool isActuallyMoving(const Player& player) const;
    std::string determineSpriteName(const Player& player) const;
    bool shouldFlipSprite(const Player& player, bool actuallyMoving) const;
    void updateAnimationState(const Player& player, bool actuallyMoving) const;

    std::string checkDamagedSprite(const Player& player) const;
    std::string checkDefenseSprite(const Player& player) const;
    std::string checkMovementSprite(const Player& player) const;
    std::string checkAttackSprite(const Player& player) const;

    std::string generateDefenseSpriteName() const;
    std::string generateMovementSpriteName() const;

    void updateMovementState(bool currentLeft, bool currentRight) const;
    void updateFrameCounter(bool actuallyMoving) const;
    void updateIdleFrames(bool currentLeft, bool currentRight) const;
};
