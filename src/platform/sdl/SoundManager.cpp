#include "SoundManager.h"
#include <iostream>
#include "AssetManager.h"

AssetManager* SoundManager::assets = nullptr;
std::set<std::string> SoundManager::playedThisFrame;
std::map<std::string, int> SoundManager::lastPlayFrame;
std::set<std::string> SoundManager::blockedSounds;
int SoundManager::currentFrame = 0;
bool SoundManager::soundEnabled = true;
float SoundManager::globalVolume = 1.0f;
bool SoundManager::initialized = false;

void SoundManager::initialize(AssetManager* a) {
    assets = a;
    initialized = true;
}

void SoundManager::shutdown() {
    playedThisFrame.clear();
    lastPlayFrame.clear();
    blockedSounds.clear();
    assets = nullptr;
    initialized = false;
}

void SoundManager::playOnce(const std::string& soundName) {
    const auto shouldPlay = [&]() {
        return initialized && soundEnabled &&
               playedThisFrame.find(soundName) == playedThisFrame.end() &&
               blockedSounds.find(soundName) == blockedSounds.end();
    };

    const auto markAndPlay = [&]() {
        playSound(soundName);
        playedThisFrame.insert(soundName);
    };

    shouldPlay() && (markAndPlay(), true);
}

void SoundManager::playWithCooldown(const std::string& soundName, int cooldownFrames) {
    const auto canPlay = [&]() {
        return initialized && soundEnabled &&
               canPlaySound(soundName, cooldownFrames) &&
               blockedSounds.find(soundName) == blockedSounds.end();
    };

    const auto playAndUpdate = [&]() {
        playSound(soundName);
        lastPlayFrame[soundName] = currentFrame;
    };

    canPlay() && (playAndUpdate(), true);
}

void SoundManager::playImmediate(const std::string& soundName) {
    std::cout << "효과음 재생 시도: " << soundName << std::endl;

    if (!initialized) {
        std::cout << "SoundManager 초기화 안됨!" << std::endl;
        return;
    }

    if (!soundEnabled) {
        std::cout << "사운드 비활성화됨!" << std::endl;
        return;
    }

    if (blockedSounds.find(soundName) != blockedSounds.end()) {
        std::cout << "차단된 효과음: " << soundName << std::endl;
        return;
    }

    playSound(soundName);
}

void SoundManager::blockSound(const std::string& soundName) {
    blockedSounds.insert(soundName);
    std::cout << "효과음 차단: " << soundName << std::endl;
}

void SoundManager::clearBlocks() {
    blockedSounds.clear();
}

void SoundManager::nextFrame() {
    playedThisFrame.clear();
    clearBlocks();
    currentFrame++;
}

void SoundManager::setEnabled(bool enabled) {
    soundEnabled = enabled;
}

void SoundManager::setVolume(float volume) {
    globalVolume = volume;
    Mix_Volume(-1, static_cast<int>(128 * globalVolume));
}

bool SoundManager::canPlaySound(const std::string& soundName, int cooldownFrames) {
    auto it = lastPlayFrame.find(soundName);
    return it == lastPlayFrame.end() || (currentFrame - it->second) >= cooldownFrames;
}

void SoundManager::playSound(const std::string& soundName) {
    if (!assets) {
        std::cout << "AssetManager가 null!" << std::endl;
        return;
    }

    Mix_Chunk* sound = assets->getSound(soundName);
    if (!sound) {
        std::cout << "효과음 로딩 실패: " << soundName << std::endl;
        return;
    }

    std::cout << "효과음 재생 성공: " << soundName << std::endl;
    Mix_PlayChannel(-1, sound, 0);
}
