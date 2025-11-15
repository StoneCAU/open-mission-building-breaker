#include "PlayerAction.h"
#include <windows.h>
#include "../../ui/InputHandler.h"

PlayerAction::PlayerAction()
    : action(PlayerActionType::IDLE),
      actionFrame(0),
      actionCooldown(0),
      canAttack(true) {}

void PlayerAction::reset() {
    action = PlayerActionType::IDLE;
    actionFrame = 0;
    actionCooldown = 0;
    canAttack = true;
}

void PlayerAction::handleInput(InputKey key) {
    if (key == InputKey::ATTACK) {
        tryAttack();
    }

    if (key == InputKey::DEFEND) {
        tryDefend();
    }
}

bool PlayerAction::tryAttack() {
    if (!canStartAttack()) {
        return false;
    }

    startAttack();
    return true;
}

bool PlayerAction::canStartAttack() const {
    return actionCooldown == 0 && canAttack;
}

void PlayerAction::startAttack() {
    action = PlayerActionType::ATTACK;
    actionFrame = ACTION_DURATION;
    actionCooldown = ATTACK_COOLDOWN;
    canAttack = false;
}

bool PlayerAction::tryDefend() {
    if (!canStartDefend()) {
        return false;
    }

    startDefend();
    return true;
}

bool PlayerAction::canStartDefend() const {
    return actionCooldown == 0;
}

void PlayerAction::startDefend() {
    action = PlayerActionType::DEFEND;
    actionFrame = 0;
}

void PlayerAction::update() {
    updateActionFrame();
    updateCooldown();
    updateDefendRelease();
}

void PlayerAction::updateActionFrame() {
    if (action == PlayerActionType::DEFEND) {
        return;
    }

    if (actionFrame > 0) {
        --actionFrame;
    }

    if (actionFrame == 0 && action == PlayerActionType::ATTACK) {
        endAction();
    }
}

void PlayerAction::updateCooldown() {
    if (actionCooldown > 0) {
        --actionCooldown;

        if (actionCooldown == 0) {
            canAttack = true;
        }
    }
}

void PlayerAction::updateDefendRelease() {
    if (action == PlayerActionType::DEFEND && isDefendKeyReleased()) {
        endAction();
    }
}

bool PlayerAction::isDefendKeyReleased() const {
    return InputHandler::isKeyReleased(VK_DOWN);
}

void PlayerAction::endAction() {
    action = PlayerActionType::IDLE;
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

bool PlayerAction::isAttackActiveFrame() const {
    return action == PlayerActionType::ATTACK &&
           actionFrame >= ATTACK_ACTIVE_START_FRAME && 
           actionFrame <= ATTACK_ACTIVE_END_FRAME;
}
