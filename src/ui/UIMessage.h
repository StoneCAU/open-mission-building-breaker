#pragma once

#include <string>

enum class MessageType {
    NONE,
    ATTACK_HIT,
    DEFENSE_SUCCESS,
    PLAYER_DAMAGED,
    ULTIMATE_ACTIVATED
};

struct UIMessage {
    MessageType type;
    int param;

    UIMessage() : type(MessageType::NONE), param(0) {}
    UIMessage(MessageType t, int p = 0) : type(t), param(p) {}

    std::string format() const;
};

class UIMessageQueue {
    static constexpr int DISPLAY_FRAMES = 60;
    UIMessage currentMessage;
    int displayFramesLeft;

public:
    UIMessageQueue() : displayFramesLeft(0) {}

    void push(MessageType type, int param = 0) {
        currentMessage = UIMessage(type, param);
        displayFramesLeft = DISPLAY_FRAMES;
    }

    void update() {
        displayFramesLeft = std::max(0, displayFramesLeft - 1);
    }

    bool hasMessage() const {
        return displayFramesLeft > 0;
    }

    std::string getMessage() const {
        return currentMessage.format();
    }

    void clear() {
        displayFramesLeft = 0;
        currentMessage = UIMessage();
    }

};
