// SoundManager.cpp
#include "SoundManager.h"
#include <iostream>
#include "AssetManager.h"

AssetManager* SoundManager::assets = nullptr;
std::set<std::string> SoundManager::playedThisFrame;
std::map<std::string, int> SoundManager::lastPlayFrame;
std::set<std::string> SoundManager::blockedSounds;
std::map<std::string, int> SoundManager::blockedUntilFrame;
int SoundManager::currentFrame = 0;
bool SoundManager::soundEnabled = true;
float SoundManager::globalVolume = 1.0f;
bool SoundManager::initialized = false;

std::string SoundManager::currentBGM = "";
bool SoundManager::bgmLoop = true;
float SoundManager::bgmVolume = 0.5f;

void SoundManager::initialize(AssetManager* a) {
    assets = a;
    initialized = true;
}

void SoundManager::shutdown() {
    playedThisFrame.clear();
    lastPlayFrame.clear();
    blockedSounds.clear();
    blockedUntilFrame.clear();
    stopBGM();
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
    if (!initialized || !soundEnabled) return;
    
    if (blockedSounds.find(soundName) != blockedSounds.end()) return;

    playSound(soundName);
}

void SoundManager::blockSound(const std::string& soundName, int frames) {
    blockedSounds.insert(soundName);
    blockedUntilFrame[soundName] = currentFrame + frames;
}

void SoundManager::clearBlocks() {
    auto it = blockedUntilFrame.begin();
    while (it != blockedUntilFrame.end()) {
        if (currentFrame >= it->second) {
            blockedSounds.erase(it->first);
            it = blockedUntilFrame.erase(it);
        } else {
            ++it;
        }
    }
}

void SoundManager::playBGM(const std::string& bgmName, bool loop) {
    if (!initialized || !assets) return;

    if (currentBGM == bgmName && Mix_PlayingMusic()) {
        return;
    }

    Mix_HaltMusic();

    Mix_Music* music = assets->getMusic(bgmName);
    if (!music) {
        return;
    }

    currentBGM = bgmName;
    bgmLoop = loop;
    Mix_PlayMusic(music, loop ? -1 : 0);
    Mix_VolumeMusic(static_cast<int>(128 * bgmVolume));
}

void SoundManager::stopBGM() {
    Mix_HaltMusic();
    currentBGM = "";
}

void SoundManager::setBGMVolume(float volume) {
    bgmVolume = volume;
    Mix_VolumeMusic(static_cast<int>(128 * bgmVolume));
}

bool SoundManager::isBGMPlaying() {
    return Mix_PlayingMusic() != 0;
}

void SoundManager::nextFrame() {
    playedThisFrame.clear();
    clearBlocks();  // 시간 기반 차단 해제
    currentFrame++;

    if (bgmLoop && !Mix_PlayingMusic() && !currentBGM.empty()) {
        playBGM(currentBGM, true);
    }
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
    if (!assets) return;

    Mix_Chunk* sound = assets->getSound(soundName);
    if (!sound) return;

    Mix_PlayChannel(-1, sound, 0);
}
