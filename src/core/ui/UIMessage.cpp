#include "UIMessage.h"

const std::unordered_map<MessageType, UIMessage::Formatter> UIMessage::formatters = {
    {MessageType::ATTACK_HIT, formatAttackHit},
    {MessageType::DEFENSE_SUCCESS, formatDefenseSuccess},
    {MessageType::PLAYER_DAMAGED, formatPlayerDamaged},
    {MessageType::ULTIMATE_ACTIVATED, formatUltimate},
    {MessageType::NONE, formatNone}
};

UIMessage::UIMessage() 
    : type(MessageType::NONE), param(0) {}

UIMessage::UIMessage(MessageType t, int p) 
    : type(t), param(p) {}

std::string UIMessage::format() const {
    auto it = formatters.find(type);
    if (it != formatters.end()) {
        return it->second(param);
    }
    return padMessage("");
}

std::string UIMessage::formatAttackHit(int param) {
    std::string message = std::string(ATTACK_HIT_PREFIX) + 
                         std::to_string(param) + 
                         ATTACK_HIT_SUFFIX;
    return padMessage(message);
}

std::string UIMessage::formatDefenseSuccess(int param) {
    return padMessage(DEFENSE_SUCCESS_MSG);
}

std::string UIMessage::formatPlayerDamaged(int param) {
    return padMessage(PLAYER_DAMAGED_MSG);
}

std::string UIMessage::formatUltimate(int param) {
    std::string message = std::string(ULTIMATE_PREFIX) + 
                         std::to_string(param) + 
                         ULTIMATE_SUFFIX;
    return padMessage(message);
}

std::string UIMessage::formatNone(int param) {
    return padMessage("");
}

std::string UIMessage::padMessage(const std::string& message) {
    std::string result = message;
    result.resize(MESSAGE_LENGTH, ' ');
    return result;
}

UIMessageQueue::UIMessageQueue() 
    : displayFramesLeft(0) {}

void UIMessageQueue::push(MessageType type, int param) {
    currentMessage = UIMessage(type, param);
    displayFramesLeft = DISPLAY_FRAMES;
}

void UIMessageQueue::update() {
    if (displayFramesLeft > 0) {
        --displayFramesLeft;
    }
}

void UIMessageQueue::clear() {
    displayFramesLeft = 0;
    currentMessage = UIMessage();
}

bool UIMessageQueue::hasMessage() const {
    return displayFramesLeft > 0;
}

std::string UIMessageQueue::getMessage() const {
    return currentMessage.format();
}
