namespace GameConfig {
    // ===== 물리 =====
    inline constexpr float GRAVITY = 0.1f;
    inline constexpr float JUMP_VELOCITY = -1.0f;
    inline constexpr float BUILDING_GRAVITY = 0.001f;
    inline constexpr float BUILDING_REBOUND_VELOCITY = -0.7f;
    inline constexpr float BUILDING_REBOUND_GRAVITY = 0.02f;

    // ===== 빌딩/층 =====
    inline constexpr int BUILDING_WIDTH = 6;
    inline constexpr int MIN_BUILDING_HEIGHT = 5;
    inline constexpr int MAX_BUILDING_HEIGHT = 8;

    // ===== 층 타입별 속성 =====
    inline constexpr int FLOOR_NORMAL_HP = 1;
    inline constexpr int FLOOR_STRONG_HP = 2;
    inline constexpr int FLOOR_WEAK_HP = 1;

    inline constexpr char FLOOR_NORMAL_CHAR = '#';
    inline constexpr char FLOOR_STRONG_CHAR = '=';
    inline constexpr char FLOOR_WEAK_CHAR = '-';

    // ===== 플레이어 =====
    inline constexpr float PLAYER_HEIGHT = 1.0f;
    inline constexpr float PLAYER_ATTACK_RANGE = 2.0f;
    inline constexpr float PLAYER_DEFENSE_RANGE = 0.5f;

    // ===== 맵 =====
    inline constexpr int MAP_WIDTH = 50;
    inline constexpr int MAP_MIN_X = 0;
    inline constexpr int MAP_MAX_X = MAP_WIDTH - 1;
    inline constexpr int MAP_GROUND_Y = 20;

    // ===== 게임 세션 =====
    inline constexpr int INITIAL_SCORE = 0;
    inline constexpr int INITIAL_COMBO = 0;
    inline constexpr int INITIAL_GAUGE = 0;
    inline constexpr int INITIAL_LIFE = 3;
    inline constexpr int SCORE_PER_ATTACK_HIT = 100;

    // ===== 액션 =====
    inline constexpr int PLAYER_ACTION_DURATION = 10;
    inline constexpr int PLAYER_ATTACK_COOLDOWN = 10;
    inline constexpr int PLAYER_INVINCIBILITY_FRAMES = 30;

    // ===== 점프 =====
    inline constexpr int PLAYER_JUMP_DURATION = 20;
    inline constexpr int PLAYER_JUMP_COOLDOWN_MAX = 15;
    inline constexpr int PLAYER_START_X = MAP_WIDTH / 2;

    // ===== 필살기 =====
    inline constexpr int ULTIMATE_GAUGE_COST = 100;

    // ===== 빌딩 스폰 =====
    inline constexpr int MIN_ONSCREEN_BUILDINGS = 3;
    inline constexpr int MAX_ONSCREEN_BUILDINGS = 5;
    inline constexpr int BUILDING_SPAWN_COOLDOWN = 80;

    // ===== 시스템 =====
    inline constexpr int FRAME_DELAY_MS = 20;
}
