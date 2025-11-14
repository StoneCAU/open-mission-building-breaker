#include "Player.h"
#include "../building/Building.h"
#include "../game/GameConfig.h"

Player::Player()
    : x(GameConfig::PLAYER_START_X),
      y(static_cast<float>(GameConfig::MAP_GROUND_Y)),
      movement(x, y),
      collision(x, y, movement, action) {}

void Player::handleInput(InputKey key) {
    if (collision.isDamaged()) return;

    action.handleInput(key);

    if (movement.handleInput(key)) {
        action.setActionCooldown(GameConfig::PLAYER_JUMP_COOLDOWN_MAX);
    }
}

void Player::update() {
    // 1. 빌딩에 붙어있으면 빌딩 따라 이동
    if (attachedBuilding) {
        // 빌딩 파괴되었으면 떼어냄
        if (attachedBuilding->isDestroyed()) {
            detachFromBuilding();
        }
        // 빌딩이 지면 닿으면 피격 + 떼어냄 (추가!)
        else if (attachedBuilding->isOnGround()) {
            if (!collision.isDamaged()) {
                collision.takeDamage();
            }
            detachFromBuilding();
        }
        // 정상: 빌딩 따라 이동
        else {
            float targetY = attachedBuilding->getBottomY() + GameConfig::PLAYER_HEIGHT;
            float targetVelocityY = attachedBuilding->getVelocityY();
            movement.followObject(targetY, targetVelocityY);
        }
    }

    action.update();

    if (!attachedBuilding) {
        movement.update();
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
    movement.handleCollisionWith(obstacleY);
}

void Player::attachToBuilding(Building* building) {
    attachedBuilding = building;
}

void Player::detachFromBuilding() {
    attachedBuilding = nullptr;
}

bool Player::isAttachedToBuilding() const {
    return attachedBuilding != nullptr;
}
