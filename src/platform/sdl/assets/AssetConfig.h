#pragma once

namespace AssetConfig {
    // 폰트 키
    constexpr const char* FONT_TITLE = "title";
    constexpr const char* FONT_MENU = "menu";
    constexpr const char* FONT_GAME = "game";

    // 배경 텍스처 키
    constexpr const char* TEXTURE_TITLE = "title";
    constexpr const char* TEXTURE_DOJO_BG = "dojo_bg";
    constexpr const char* TEXTURE_GAME_BG = "game_bg";
    constexpr const char* TEXTURE_GAME_OVER_BG = "game_over_bg";

    // UI 텍스처 키
    constexpr const char* TEXTURE_SCORE_FRAME = "score_frame";
    constexpr const char* TEXTURE_COMBO_MEDAL = "combo_medal";
    constexpr const char* TEXTURE_GAUGE_EMPTY = "gauge_empty";
    constexpr const char* TEXTURE_GAUGE_FILL = "gauge_fill";
    constexpr const char* TEXTURE_LIFE_ACTIVE = "life_active";
    constexpr const char* TEXTURE_LIFE_INACTIVE = "life_inactive";
    constexpr const char* TEXTURE_HURRICANE_EFFECT = "hurricane_effect";

    // 건물 텍스처 키
    constexpr const char* TEXTURE_BUILDING_TOP = "building_top";
    constexpr const char* TEXTURE_FLOOR_NORMAL = "floor_normal";
    constexpr const char* TEXTURE_FLOOR_DESTRUCTION = "floor_destruction";

    // BGM 키
    constexpr const char* MUSIC_MENU = "menu";
    constexpr const char* MUSIC_GAME = "game";
    constexpr const char* MUSIC_GAMEOVER = "gameover";

    // 효과음 키
    constexpr const char* SOUND_MENU_SELECT = "menu_select";
    constexpr const char* SOUND_ATTACK = "attack";
    constexpr const char* SOUND_DEFEND = "defend";
    constexpr const char* SOUND_HIT = "hit";
    constexpr const char* SOUND_SPECIAL = "special";
    constexpr const char* SOUND_FLOOR_BREAK = "floor_break";
    constexpr const char* SOUND_BUILDING_COLLAPSE = "building_collapse";
}
