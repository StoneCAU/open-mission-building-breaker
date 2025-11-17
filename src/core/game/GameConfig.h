namespace GameConfig {
    // ===== 물리 =====
    inline constexpr float GRAVITY = 0.1f;
    inline constexpr float JUMP_VELOCITY = -1.0f;

    // ===== 맵 =====
    inline constexpr int MAP_WIDTH = 50;
    inline constexpr int MAP_MIN_X = 0;
    inline constexpr int MAP_MAX_X = 35;
    inline constexpr int MAP_GROUND_Y = 20;

    // ===== 게임 세션 =====
    inline constexpr int INITIAL_SCORE = 0;
    inline constexpr int INITIAL_COMBO = 0;
    inline constexpr int INITIAL_GAUGE = 0;
    inline constexpr int INITIAL_LIFE = 3;
    inline constexpr int SCORE_PER_ATTACK_HIT = 100;

    // ===== 점프 =====
    inline constexpr int PLAYER_JUMP_DURATION = 20;

    // ===== 필살기 =====
    inline constexpr int ULTIMATE_GAUGE_COST = 100;

    // ===== 빌딩 스폰 =====
    inline constexpr int MIN_ONSCREEN_BUILDINGS = 3;
    inline constexpr int MAX_ONSCREEN_BUILDINGS = 5;
    inline constexpr int BUILDING_SPAWN_COOLDOWN = 80;

    // ===== 시스템 =====
    inline constexpr int FRAME_DELAY_MS = 20;
}
