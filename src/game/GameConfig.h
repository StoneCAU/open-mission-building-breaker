#pragma once

namespace GameConfig {
    // ===== 맵 관련 =====
    inline constexpr int MAP_WIDTH = 21;
    inline constexpr int MAP_MIN_X = 0;
    inline constexpr int MAP_MAX_X = MAP_WIDTH - 1;
    inline constexpr int MAP_GROUND_Y = 10;

    // ===== 플레이어 기본 스탯 =====
    inline constexpr int PLAYER_JUMP_HEIGHT = 2;
    inline constexpr int PLAYER_JUMP_DURATION = 3;
    inline constexpr int PLAYER_START_X = MAP_WIDTH / 2;
}