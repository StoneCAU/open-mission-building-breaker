#include "AssetManager.h"
#include <SDL2/SDL_image.h>
#include <vector>

AssetManager::AssetManager(SDL_Renderer* r) : renderer(r) {}

AssetManager::~AssetManager() {
    for (auto& [name, font] : fonts) {
        TTF_CloseFont(font);
    }
    for (auto& [name, texture] : textures) {
        SDL_DestroyTexture(texture);
    }
    for (auto& [name, bgm] : music) {
        Mix_FreeMusic(bgm);
    }
    for (auto& [name, sound] : sounds) {
        Mix_FreeChunk(sound);
    }
}

bool AssetManager::loadFonts() {
    TTF_Font* title = TTF_OpenFont("assets/fonts/PressStart2P-Regular.ttf", TITLE_FONT_SIZE);
    TTF_Font* menu = TTF_OpenFont("assets/fonts/PressStart2P-Regular.ttf", MENU_FONT_SIZE);
    TTF_Font* game = TTF_OpenFont("assets/fonts/DungGeunMo.ttf", GAME_FONT_SIZE);

    if (!title) title = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", TITLE_FONT_SIZE);
    if (!menu) menu = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", MENU_FONT_SIZE);
    if (!game) game = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", FALLBACK_GAME_FONT_SIZE);

    if (!title || !menu || !game) return false;

    fonts["title"] = title;
    fonts["menu"] = menu;
    fonts["game"] = game;

    return true;
}

bool AssetManager::loadTextures() {
    return loadBackgroundTextures() &&
           loadUITextures() &&
           loadNumberTextures() &&
           loadPlayerTextures() &&
           loadBuildingTextures();
}

bool AssetManager::loadBackgroundTextures() {
    const std::vector<std::pair<std::string, std::string>> backgroundTextures = {
        {"title", "assets/images/title_samurai.png"},
        {"dojo_bg", "assets/images/dojo_background.png"},
        {"game_bg", "assets/images/samurai_game_background.png"},
        {"game_over_bg", "assets/images/game_over_background.png"}
    };

    loadTextureGroup(backgroundTextures);
    return true;
}

bool AssetManager::loadUITextures() {
    const std::vector<std::pair<std::string, std::string>> uiTextures = {
        {"ui_panel", "assets/images/ui_top_panel.png"},
        {"score_frame", "assets/images/score_frame.png"},
        {"combo_medal", "assets/images/combo_medal.png"},
        {"special_gauge", "assets/images/special_gauge_frame.png"},
        {"gauge_empty", "assets/images/gauge_empty.png"},
        {"gauge_fill", "assets/images/gauge_fill.png"},
        {"life_active", "assets/images/life_active.png"},
        {"life_inactive", "assets/images/life_inactive.png"},
        {"hurricane_effect", "assets/images/hurricane_effect.png"}
    };

    loadTextureGroup(uiTextures);
    return true;
}

bool AssetManager::loadNumberTextures() {
    const std::vector<std::pair<std::string, std::string>> numberTextures = {
        {"number_0", "assets/images/number_0.png"},
        {"number_1", "assets/images/number_1.png"},
        {"number_2", "assets/images/number_2.png"},
        {"number_3", "assets/images/number_3.png"},
        {"number_4", "assets/images/number_4.png"},
        {"number_5", "assets/images/number_5.png"},
        {"number_6", "assets/images/number_6.png"},
        {"number_7", "assets/images/number_7.png"},
        {"number_8", "assets/images/number_8.png"},
        {"number_9", "assets/images/number_9.png"}
    };

    loadTextureGroup(numberTextures);
    return true;
}

bool AssetManager::loadPlayerTextures() {
    const std::vector<std::pair<std::string, std::string>> playerTextures = {
        {"player_idle", "assets/sprites/player/idle.png"},
        {"player_move_1", "assets/sprites/player/move_1.png"},
        {"player_move_2", "assets/sprites/player/move_2.png"},
        {"player_move_3", "assets/sprites/player/move_3.png"},
        {"player_attack", "assets/sprites/player/attack.png"},
        {"player_defence_1", "assets/sprites/player/defence_1.png"},
        {"player_defence_2", "assets/sprites/player/defence_2.png"},
        {"player_hit", "assets/sprites/player/hit.png"}
    };

    loadTextureGroup(playerTextures);
    return true;
}

bool AssetManager::loadBuildingTextures() {
    const std::vector<std::pair<std::string, std::string>> buildingTextures = {
        {"building_top", "assets/sprites/buildings/building_top.png"},
        {"floor_normal", "assets/sprites/buildings/floor_normal.png"},
        {"floor_destruction", "assets/sprites/buildings/floor_destruction.png"}
    };

    loadTextureGroup(buildingTextures);
    return true;
}

void AssetManager::loadTextureGroup(const std::vector<std::pair<std::string, std::string>>& textureList) {
    for (const auto& [key, path] : textureList) {
        loadSingleTexture(key, path);
    }
}

bool AssetManager::loadSingleTexture(const std::string& key, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture) {
        textures[key] = texture;
        return true;
    }

    return false;
}

bool AssetManager::loadAudio() {
    Mix_Music* menuMusic = Mix_LoadMUS("assets/audio/bgm/menu_bgm.mp3");
    Mix_Music* gameMusic = Mix_LoadMUS("assets/audio/bgm/game_bgm.mp3");
    Mix_Music* gameoverMusic = Mix_LoadMUS("assets/audio/bgm/gameover_bgm.mp3");

    menuMusic && (music["menu"] = menuMusic, true);
    gameMusic && (music["game"] = gameMusic, true);
    gameoverMusic && (music["gameover"] = gameoverMusic, true);

    sounds["menu_select"] = Mix_LoadWAV("assets/audio/sfx/menu_select.wav");
    sounds["attack"] = Mix_LoadWAV("assets/audio/sfx/attack.mp3");
    sounds["defend"] = Mix_LoadWAV("assets/audio/sfx/defend.mp3");
    sounds["hit"] = Mix_LoadWAV("assets/audio/sfx/hit.mp3");
    sounds["special"] = Mix_LoadWAV("assets/audio/sfx/special_attack.mp3");
    sounds["floor_break"] = Mix_LoadWAV("assets/audio/sfx/floor_break.wav");
    sounds["building_collapse"] = Mix_LoadWAV("assets/audio/sfx/building_collapse.mp3");

    return menuMusic && gameMusic && gameoverMusic;
}

TTF_Font* AssetManager::getFont(const std::string& name) {
    auto it = fonts.find(name);
    TTF_Font* result = nullptr;

    (it != fonts.end()) && (result = it->second, true);

    return result;
}

SDL_Texture* AssetManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    SDL_Texture* result = nullptr;

    (it != textures.end()) && (result = it->second, true);

    return result;
}

Mix_Music* AssetManager::getMusic(const std::string& name) {
    auto it = music.find(name);
    Mix_Music* result = nullptr;

    (it != music.end()) && (result = it->second, true);

    return result;
}

Mix_Chunk* AssetManager::getSound(const std::string& name) {
    auto it = sounds.find(name);
    Mix_Chunk* result = nullptr;

    (it != sounds.end()) && (result = it->second, true);

    return result;
}
