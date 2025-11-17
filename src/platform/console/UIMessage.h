#pragma once

#include <string>
#include <functional>
#include <unordered_map>

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

    UIMessage();
    UIMessage(MessageType t, int p = 0);

    std::string format() const;

private:
    static constexpr int MESSAGE_LENGTH = 50;
    static constexpr const char* ATTACK_HIT_PREFIX = "[HIT!] ";
    static constexpr const char* ATTACK_HIT_SUFFIX = "개 층 파괴! 콤보 +1";
    static constexpr const char* DEFENSE_SUCCESS_MSG = "[방어 성공!] 건물을 막아냈습니다!";
    static constexpr const char* PLAYER_DAMAGED_MSG = "[충돌!] 목숨 -1, 콤보 초기화!";
    static constexpr const char* ULTIMATE_PREFIX = "[필살기!] ";
    static constexpr const char* ULTIMATE_SUFFIX = "개 건물 파괴!";

    using Formatter = std::function<std::string(int)>;
    static const std::unordered_map<MessageType, Formatter> formatters;

    static std::string formatAttackHit(int param);
    static std::string formatDefenseSuccess(int param);
    static std::string formatPlayerDamaged(int param);
    static std::string formatUltimate(int param);
    static std::string formatNone(int param);

    static std::string padMessage(const std::string& message);
};

class UIMessageQueue {
public:
    UIMessageQueue();

    void push(MessageType type, int param = 0);
    void update();
    void clear();

    bool hasMessage() const;
    std::string getMessage() const;

private:
    static constexpr int DISPLAY_FRAMES = 60;

    UIMessage currentMessage;
    int displayFramesLeft;
};
