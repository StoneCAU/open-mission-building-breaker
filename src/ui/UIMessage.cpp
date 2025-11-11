#include "UIMessage.h"

#include <string>

namespace {
    // ====== 메시지 상수 ======
    constexpr const char* MSG_ATTACK_HIT_PREFIX = "[HIT!] ";
    constexpr const char* MSG_ATTACK_HIT_SUFFIX = "개 층 파괴! 콤보 +1";
    constexpr const char* MSG_DEFENSE_SUCCESS = "[방어 성공!] 건물을 막아냈습니다!";
    constexpr const char* MSG_PLAYER_DAMAGED = "[충돌!] 목숨 -1, 콤보 초기화!";
}

std::string UIMessage::format() const {
    std::string result;

    if (type == MessageType::ATTACK_HIT) {
        result = std::string(MSG_ATTACK_HIT_PREFIX) + std::to_string(param) + MSG_ATTACK_HIT_SUFFIX;
    }
    if (type == MessageType::DEFENSE_SUCCESS) {
        result = MSG_DEFENSE_SUCCESS;
    }
    if (type == MessageType::PLAYER_DAMAGED) {
        result = MSG_PLAYER_DAMAGED;
    }

    result.resize(50, ' ');
    return result;
}
