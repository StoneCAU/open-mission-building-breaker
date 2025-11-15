#pragma once

#include "PlayerActionType.h"
#include "../../interfaces/InputKey.h"

class PlayerAction {
public:
    PlayerAction();

    void reset();
    void handleInput(InputKey key);
    void update(bool defendKeyReleased);

    PlayerActionType getType() const;
    int getActionCooldown() const;
    void setActionCooldown(int value);
    bool isAttackActiveFrame() const;

private:
    static constexpr int ACTION_DURATION = 10;
    static constexpr int ATTACK_COOLDOWN = 10;
    static constexpr int ATTACK_ACTIVE_START_FRAME = 5;
    static constexpr int ATTACK_ACTIVE_END_FRAME = 7;

    PlayerActionType action;
    int actionFrame;
    int actionCooldown;
    bool canAttack;

    bool tryAttack();
    bool tryDefend();

    void updateActionFrame();
    void updateCooldown();

    bool canStartAttack() const;
    bool canStartDefend() const;
    void startAttack();
    void startDefend();
    void endAction();
};
