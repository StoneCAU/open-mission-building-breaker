#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    AssetManager(SDL_Renderer* renderer);
    ~AssetManager();

    bool loadFonts();
    bool loadTextures();
    bool loadSingleTexture(const std::string& key, const std::string& path);
    
    TTF_Font* getFont(const std::string& name);
    SDL_Texture* getTexture(const std::string& name);

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, TTF_Font*> fonts;
    std::unordered_map<std::string, SDL_Texture*> textures;
};
