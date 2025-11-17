#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>
#include <set>
#include <map>

class AssetManager;

class SoundManager {
public:
    static void initialize(AssetManager* assets);
    static void shutdown();

    static void playOnce(const std::string& soundName);
    static void playWithCooldown(const std::string& soundName, int cooldownFrames);
    static void playImmediate(const std::string& soundName);

    static void blockSound(const std::string& soundName);
    static void clearBlocks();

    static void playBGM(const std::string& bgmName, bool loop = true);
    static void stopBGM();
    static void setBGMVolume(float volume);
    static bool isBGMPlaying();

    static void nextFrame();
    static void setEnabled(bool enabled);
    static void setVolume(float volume);

private:
    static AssetManager* assets;
    static std::set<std::string> playedThisFrame;
    static std::map<std::string, int> lastPlayFrame;
    static std::set<std::string> blockedSounds;
    static int currentFrame;
    static bool soundEnabled;
    static float globalVolume;
    static bool initialized;

    static std::string currentBGM;
    static bool bgmLoop;
    static float bgmVolume;

    static bool canPlaySound(const std::string& soundName, int cooldownFrames);
    static void playSound(const std::string& soundName);
};
