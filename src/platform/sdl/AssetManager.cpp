#include "AssetManager.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

AssetManager::AssetManager(SDL_Renderer* r) : renderer(r) {}

AssetManager::~AssetManager() {
    for (auto& [name, font] : fonts) {
        TTF_CloseFont(font);
    }
    for (auto& [name, texture] : textures) {
        SDL_DestroyTexture(texture);
    }
}

bool AssetManager::loadFonts() {
    TTF_Font* title = TTF_OpenFont("assets/fonts/PressStart2P-Regular.ttf", 16);
    TTF_Font* menu = TTF_OpenFont("assets/fonts/PressStart2P-Regular.ttf", 12);
    TTF_Font* game = TTF_OpenFont("assets/fonts/PressStart2P-Regular.ttf", 14);

    if (!title) title = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 16);
    if (!menu) menu = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 12);
    if (!game) game = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 14);

    if (!title || !menu || !game) return false;

    fonts["title"] = title;
    fonts["menu"] = menu;
    fonts["game"] = game;

    return true;
}

bool AssetManager::loadTextures() {
    // 텍스처 목록 정의
    std::pmr::vector<std::pair<std::string, std::string>> textureList = {
        {"title", "assets/images/title_samurai.png"},
        {"dojo_bg", "assets/images/dojo_background.png"},
    };

    for (const auto& [key, path] : textureList) {
        loadSingleTexture(key, path);
    }

    return true;
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

TTF_Font* AssetManager::getFont(const std::string& name) {
    auto it = fonts.find(name);
    return (it != fonts.end()) ? it->second : nullptr;
}

SDL_Texture* AssetManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    return (it != textures.end()) ? it->second : nullptr;
}
