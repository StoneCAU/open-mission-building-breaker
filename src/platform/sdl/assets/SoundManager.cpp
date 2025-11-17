#include "SoundManager.h"
#include "AssetManager.h"

AssetManager* SoundManager::assets = nullptr;
std::set<std::string> SoundManager::playedThisFrame;
std::map<std::string, int> SoundManager::lastPlayFrame;
std::set<std::string> SoundManager::blockedSounds;
std::map<std::string, int> SoundManager::blockedUntilFrame;
int SoundManager::currentFrame = 0;
bool SoundManager::soundEnabled = true;
float SoundManager::globalVolume = DEFAULT_GLOBAL_VOLUME;
bool SoundManager::initialized = false;

std::string SoundManager::currentBGM = "";
bool SoundManager::bgmLoop = true;
float SoundManager::bgmVolume = DEFAULT_BGM_VOLUME;

void SoundManager::initialize(AssetManager* a) {
    assets = a;
    initialized = true;
    Mix_AllocateChannels(MAX_AUDIO_CHANNELS);
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
    const auto canPlayImmediate = [&]() {
        return initialized && soundEnabled &&
               blockedSounds.find(soundName) == blockedSounds.end();
    };

    canPlayImmediate() && (playSound(soundName), true);
}

void SoundManager::blockSound(const std::string& soundName, int frames) {
    blockedSounds.insert(soundName);
    blockedUntilFrame[soundName] = currentFrame + frames;
}

void SoundManager::clearBlocks() {
    auto it = blockedUntilFrame.begin();

    const auto processNextBlock = [&]() {
        const auto& [soundName, frameLimit] = *it;
        processBlockedSound(soundName, frameLimit);
        return it != blockedUntilFrame.end();
    };

    it != blockedUntilFrame.end() && (processNextBlock(), true);
}

void SoundManager::processBlockedSound(const std::string& soundName, int frameLimit) {
    auto it = blockedUntilFrame.find(soundName);

    const auto removeExpiredBlock = [&]() {
        blockedSounds.erase(soundName);
        blockedUntilFrame.erase(it);
    };

    const auto moveToNext = [&]() {
        ++it;
    };

    isBlockExpired(frameLimit) && (removeExpiredBlock(), true) || (moveToNext(), true);
}

bool SoundManager::isBlockExpired(int frameLimit) {
    return currentFrame >= frameLimit;
}

void SoundManager::playBGM(const std::string& bgmName, bool loop) {
    const auto processBGMRequest = [&]() {
        stopCurrentBGM();
        loadAndPlayBGM(bgmName, loop) && (updateBGMSettings(bgmName, loop), true);
    };

    shouldPlayBGM(bgmName) && (processBGMRequest(), true);
}

bool SoundManager::shouldPlayBGM(const std::string& bgmName) {
    return initialized && assets && !isSameBGMPlaying(bgmName);
}

bool SoundManager::isSameBGMPlaying(const std::string& bgmName) {
    return currentBGM == bgmName && Mix_PlayingMusic();
}

void SoundManager::stopCurrentBGM() {
    Mix_HaltMusic();
}

bool SoundManager::loadAndPlayBGM(const std::string& bgmName, bool loop) {
    Mix_Music* music = assets->getMusic(bgmName);

    const auto playMusic = [&]() {
        int loopCount = loop ? INFINITE_LOOP : NO_LOOP;
        Mix_PlayMusic(music, loopCount);
        Mix_VolumeMusic(calculateSDLVolume(bgmVolume));
    };

    return music && (playMusic(), true);
}

void SoundManager::updateBGMSettings(const std::string& bgmName, bool loop) {
    currentBGM = bgmName;
    bgmLoop = loop;
}

void SoundManager::stopBGM() {
    Mix_HaltMusic();
    currentBGM = "";
}

void SoundManager::setBGMVolume(float volume) {
    bgmVolume = volume;
    Mix_VolumeMusic(calculateSDLVolume(bgmVolume));
}

bool SoundManager::isBGMPlaying() {
    return Mix_PlayingMusic() != 0;
}

void SoundManager::nextFrame() {
    playedThisFrame.clear();
    clearBlocks();
    currentFrame++;

    const auto restartBGM = [&]() {
        playBGM(currentBGM, true);
    };

    bgmLoop && !Mix_PlayingMusic() && !currentBGM.empty() && (restartBGM(), true);
}

void SoundManager::setEnabled(bool enabled) {
    soundEnabled = enabled;
}

void SoundManager::setVolume(float volume) {
    globalVolume = volume;
    Mix_Volume(ANY_CHANNEL, calculateSDLVolume(globalVolume));
}

bool SoundManager::canPlaySound(const std::string& soundName, int cooldownFrames) {
    auto it = lastPlayFrame.find(soundName);
    return it == lastPlayFrame.end() || (currentFrame - it->second) >= cooldownFrames;
}

void SoundManager::playSound(const std::string& soundName) {
    const auto playAudioChunk = [&]() {
        Mix_Chunk* sound = assets->getSound(soundName);

        const auto playOnChannel = [&]() {
            Mix_PlayChannel(ANY_CHANNEL, sound, NO_LOOP);
        };

        sound && (playOnChannel(), true);
    };

    assets && (playAudioChunk(), true);
}

int SoundManager::calculateSDLVolume(float volume) {
    return static_cast<int>(SDL_VOLUME_MAX * volume);
}
