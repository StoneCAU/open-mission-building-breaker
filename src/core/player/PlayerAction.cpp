#include "PlayerAction.h"
#include <windows.h>

#include "../../ui/InputHandler.h"
#include "../game/GameConfig.h"

PlayerAction::PlayerAction()
    : action(PlayerActionType::IDLE),
      actionFrame(0),
      actionCooldown(0),
      canAttack(true),
      canDefend(true) {}

void PlayerAction::handleInput(InputKey key) {
    if (key == InputKey::ATTACK) {
        tryAttack();
    }

    if (key == InputKey::DEFEND) {
        tryDefend();
    }
}

bool PlayerAction::tryAttack() {
    if (actionCooldown > 0) return false;
    if (!canAttack) return false;

    action = PlayerActionType::ATTACK;
    actionFrame = GameConfig::PLAYER_ACTION_DURATION;
    canAttack = false;
    return true;
}

bool PlayerAction::tryDefend() {
    if (actionCooldown > 0) return false;

    action = PlayerActionType::DEFEND;
    actionFrame = 0;
    return true;
}

void PlayerAction::update() {
    updateActionFrame();
    updateKeyRelease();

    if (actionCooldown > 0) {
        --actionCooldown;
    }
}

void PlayerAction::updateActionFrame() {
    if (action == PlayerActionType::DEFEND) return;

    if (actionFrame > 0) {
        --actionFrame;
    }

    if (actionFrame == 0 && action == PlayerActionType::ATTACK) {
        action = PlayerActionType::IDLE;
    }
}

void PlayerAction::updateKeyRelease() {
    if (InputHandler::isKeyReleased('Z')) {
        canAttack = true;
    }

    if (InputHandler::isKeyReleased(VK_DOWN) && action == PlayerActionType::DEFEND) {
        action = PlayerActionType::IDLE;
    }
}

PlayerActionType PlayerAction::getType() const {
    return action;
}

int PlayerAction::getActionCooldown() const {
    return actionCooldown;
}

void PlayerAction::setActionCooldown(int value) {
    actionCooldown = value;
}
