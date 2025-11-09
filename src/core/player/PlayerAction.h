#pragma once
#include "PlayerActionType.h"
#include "../../ui/InputHandler.h"

class PlayerAction {
public:
    PlayerAction();

    bool tryAttack(InputKey key);
    bool tryDefend(InputKey key);
    void update();

    PlayerActionType getType() const;
    int getActionCooldown() const;
    void setActionCooldown(int value);

private:
    PlayerActionType action;
    int actionFrame;
    int actionCooldown;

    bool canAttack;
    bool canDefend;

    void updateActionFrame();
    void updateKeyRelease();
};
