#pragma once
#include "PlayerActionType.h"
#include "../../ui/InputHandler.h"

class PlayerAction {
public:
    PlayerAction();

    void handleInput(InputKey key);
    void update();

    PlayerActionType getType() const;
    int getActionCooldown() const;
    void setActionCooldown(int value);
    bool isAttackFirstFrame() const;

private:
    PlayerActionType action;
    int actionFrame;
    int actionCooldown;

    bool canAttack;
    bool canDefend;

    bool tryAttack();
    bool tryDefend();

    void updateActionFrame();
    void updateKeyRelease();
};
