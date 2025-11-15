#include "PlayerAttachment.h"

PlayerAttachment::PlayerAttachment()
    : attachedBuilding(nullptr) {}

void PlayerAttachment::attach(Building* building) {
    attachedBuilding = building;
}

void PlayerAttachment::detach() {
    attachedBuilding = nullptr;
}

bool PlayerAttachment::isAttached() const {
    return attachedBuilding != nullptr;
}

Building* PlayerAttachment::getBuilding() const {
    return attachedBuilding;
}
