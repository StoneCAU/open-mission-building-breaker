#pragma once
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

    static void blockSound(const std::string& soundName, int frames = 30);
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
    static std::map<std::string, int> blockedUntilFrame;
    static int currentFrame;
    static bool soundEnabled;
    static float globalVolume;
    static bool initialized;

    static std::string currentBGM;
    static bool bgmLoop;
    static float bgmVolume;

    static constexpr int MAX_AUDIO_CHANNELS = 32;
    static constexpr int SDL_VOLUME_MAX = 128;
    static constexpr int INFINITE_LOOP = -1;
    static constexpr int NO_LOOP = 0;
    static constexpr int ANY_CHANNEL = -1;
    static constexpr float DEFAULT_BGM_VOLUME = 0.5f;
    static constexpr float DEFAULT_GLOBAL_VOLUME = 1.0f;

    static bool canPlaySound(const std::string& soundName, int cooldownFrames);
    static void playSound(const std::string& soundName);

    static bool shouldPlayBGM(const std::string& bgmName);
    static bool isSameBGMPlaying(const std::string& bgmName);
    static void stopCurrentBGM();
    static bool loadAndPlayBGM(const std::string& bgmName, bool loop);
    static void updateBGMSettings(const std::string& bgmName, bool loop);

    static void processBlockedSound(const std::string& soundName, int frameLimit);
    static bool isBlockExpired(int frameLimit);

    static int calculateSDLVolume(float volume);
};
