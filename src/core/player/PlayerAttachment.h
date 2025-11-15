#pragma once

class Building;

class PlayerAttachment {
public:
    PlayerAttachment();

    void attach(Building* building);
    void detach();
    bool isAttached() const;
    Building* getBuilding() const;

private:
    Building* attachedBuilding;
};
