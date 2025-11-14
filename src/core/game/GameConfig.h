#pragma once

namespace GameConfig {
    // ===== 게임 세션 초기값 =====
    inline constexpr int INITIAL_SCORE = 0;
    inline constexpr int INITIAL_COMBO = 0;
    inline constexpr int INITIAL_GAUGE = 0;
    inline constexpr int INITIAL_LIFE = 3;

    // ===== 점수 관련 =====
    inline constexpr int SCORE_PER_ATTACK_HIT = 100;

    // ===== 맵 관련 =====
    inline constexpr int MAP_WIDTH = 50;
    inline constexpr int MAP_MIN_X = 0;
    inline constexpr int MAP_MAX_X = MAP_WIDTH - 1;
    inline constexpr int MAP_GROUND_Y = 20;

    // ===== 빌딩 관련 =====
    inline constexpr int BUILDING_WIDTH = 6;
    inline constexpr int MIN_BUILDING_HEIGHT = 5;
    inline constexpr int MAX_BUILDING_HEIGHT = 8;
    inline constexpr int MIN_ONSCREEN_BUILDINGS = 3;
    inline constexpr int MAX_ONSCREEN_BUILDINGS = 5;
    inline constexpr int BUILDING_SPAWN_COOLDOWN = 80;
    inline constexpr float BUILDING_FALL_SPEED_PER_FRAME = 0.15f;

    // ===== 빌딩 물리/반응 =====
    inline constexpr int BUILDING_REBOUND_DURATION_FRAMES = 10;
    inline constexpr int BUILDING_REBOUND_DECAY_PER_FRAME = 1;
    inline constexpr float BUILDING_REBOUND_SPEED_PER_FRAME = 1.0f;
    inline constexpr int BUILDING_RECOVER_DURATION_FRAMES = 60;

    // ===== 플레이어 기본 스탯 =====
    inline constexpr float PLAYER_HEIGHT = 1.0f;
    inline constexpr int PLAYER_JUMP_HEIGHT = 5;
    inline constexpr int PLAYER_JUMP_DURATION = 20;
    inline constexpr int PLAYER_JUMP_COOLDOWN_MAX = 15;
    inline constexpr int PLAYER_START_X = MAP_WIDTH / 2;

    // ===== 플레이어 피격 관련 =====
    inline constexpr int PLAYER_INVINCIBILITY_FRAMES = 30;

    // ===== 액션 관련 =====
    inline constexpr int PLAYER_ACTION_DURATION = 10;
    inline constexpr float PLAYER_ATTACK_RANGE = 1.0f;
    inline constexpr float PLAYER_DEFENSE_RANGE = 0.5f;
    inline constexpr int PLAYER_ATTACK_COOLDOWN = 10;
    inline constexpr int ULTIMATE_GAUGE_COST = 100;

    // ===== 시스템 관련 =====
    inline constexpr int FRAME_DELAY_MS = 20;
}