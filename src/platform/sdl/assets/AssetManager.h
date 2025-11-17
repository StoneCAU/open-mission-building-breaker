#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <vector>

class AssetManager {
public:
    AssetManager(SDL_Renderer* renderer);
    ~AssetManager();

    bool loadFonts();
    bool loadTextures();
    bool loadAudio();

    TTF_Font* getFont(const std::string& name);
    SDL_Texture* getTexture(const std::string& name);
    Mix_Music* getMusic(const std::string& name);
    Mix_Chunk* getSound(const std::string& name);

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, TTF_Font*> fonts;
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, Mix_Music*> music;
    std::unordered_map<std::string, Mix_Chunk*> sounds;

    static constexpr int TITLE_FONT_SIZE = 16;
    static constexpr int MENU_FONT_SIZE = 12;
    static constexpr int GAME_FONT_SIZE = 20;
    static constexpr int FALLBACK_GAME_FONT_SIZE = 14;

    bool loadBackgroundTextures();
    bool loadUITextures();
    bool loadNumberTextures();
    bool loadPlayerTextures();
    bool loadBuildingTextures();

    void loadTextureGroup(const std::vector<std::pair<std::string, std::string>>& textureList);  // 이 메서드가 필요함
    bool loadSingleTexture(const std::string& key, const std::string& path);
};
