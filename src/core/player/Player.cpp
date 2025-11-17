#include "Player.h"
#include "../building/Building.h"
#include "../game/GameConfig.h"
#include <windows.h>

Player::Player()
    : x(START_X),
      y(static_cast<float>(GameConfig::MAP_GROUND_Y)),
      movement(x, y) {}

void Player::reset() {
    movement.reset();
    action.reset();
    collision.reset();
    attachment.detach();
}

void Player::handleInput(InputKey key) {
    if (collision.isDamaged()) {
        return;
    }

    handleActionInput(key);
    handleJumpInput(key);
}

void Player::handleActionInput(InputKey key) {
    action.handleInput(key);
}

void Player::handleJumpInput(InputKey key) {
    if (movement.handleInput(key)) {
        action.setActionCooldown(JUMP_ACTION_COOLDOWN);
    }
}

void Player::update(IInputHandler* inputHandler) {
    updateAttachedState();
    updateComponents(inputHandler);
}

void Player::updateAttachedState() {
    if (!attachment.isAttached()) {
        return;
    }

    Building* building = attachment.getBuilding();

    if (shouldDetachFromBuilding(building)) {
        handleBuildingDetachment(building);
        return;
    }

    followAttachedBuilding(building);
}

bool Player::shouldDetachFromBuilding(Building* building) const {
    return building->isDestroyed() || building->isOnGround();
}

void Player::handleBuildingDetachment(Building* building) {
    if (building->isOnGround() && !collision.isDamaged()) {
        collision.takeDamage();
    }
    attachment.detach();
}

void Player::followAttachedBuilding(Building* building) {
    float targetY = building->getBottomY() + HEIGHT;
    float targetVelocityY = building->getVelocityY();
    movement.followObject(targetY, targetVelocityY);
}

void Player::updateComponents(IInputHandler* inputHandler) {
    bool jumpKeyReleased = inputHandler->isKeyReleased(VK_UP);
    bool defendKeyReleased = inputHandler->isKeyReleased(VK_DOWN);

    action.update(defendKeyReleased);

    if (!attachment.isAttached()) {
        movement.update(jumpKeyReleased);
    }

    collision.update();
}

void Player::takeDamage() {
    collision.takeDamage();
}

int Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

float Player::getVelocityY() const {
    return movement.getVelocityY();
}

PlayerActionType Player::getAction() const {
    return action.getType();
}

bool Player::isDamaged() const {
    return collision.isDamaged();
}

bool Player::isAttackActiveFrame() const {
    return action.isAttackActiveFrame();
}

bool Player::isJumping() const {
    return movement.isJumping();
}

void Player::handlePhysicsCollision(float obstacleY) {
    movement.handleCollisionWith(obstacleY, HEIGHT);
}

void Player::attachToBuilding(Building* building) {
    attachment.attach(building);
}

void Player::detachFromBuilding() {
    attachment.detach();
}

bool Player::isAttachedToBuilding() const {
    return attachment.isAttached();
}

bool Player::isMovingLeft() const {
    return movement.isMovingLeft();
}

bool Player::isMovingRight() const {
    return movement.isMovingRight();
}
