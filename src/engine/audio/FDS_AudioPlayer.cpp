#include "FDS_AudioPlayer.h"
#include "engine/resource/FDS_ResourceManager.h"

#include "SDL3_mixer/SDL_mixer.h"
#include "glm/common.hpp"
#include "spdlog/spdlog.h"

namespace fds
{
    AudioPlayer::~AudioPlayer() = default;

    AudioPlayer::AudioPlayer(fds::ResourceManager *resource_manager)
        : resource_manager_(resource_manager)
    {
        if (!resource_manager_)
        {
            throw std::runtime_error("Failed to initialize AudioPlayer: ResourceManager is null");
        }
    }

    int AudioPlayer::playChunk(std::string_view Chunk_path, int channel)
    {

        Mix_Chunk *chunk = resource_manager_->getChunk(Chunk_path);
        if (!chunk)
        {
            spdlog::error("AudioPlayer: Failed to load chunk '{}", Chunk_path);
            return -1;
        }

        int played_channel = Mix_PlayChannel(channel, chunk, 0);
        if (played_channel == -1)
        {
            spdlog::error("AudioPlayer: Failed to play chunk '{}': {}", Chunk_path, SDL_GetError());
        }
        else
        {
            spdlog::trace("AudioPlayer: Playing chunk '{}' in channel {}", Chunk_path, played_channel);
        }
        return played_channel;
    }

    bool AudioPlayer::playMusic(std::string_view music_path, int loops, int fade_in_ms)
    {
        if (music_path == current_music_)
            return true;
        current_music_ = music_path;
        Mix_Music *music = resource_manager_->getMusic(music_path);
        if (!music)
        {
            spdlog::error("Failed to load music '{}", music_path);
            return false;
        }

        Mix_HaltMusic();

        bool result = false;
        if (fade_in_ms > 0)
        {
            result = Mix_FadeInMusic(music, loops, fade_in_ms);
        }
        else
        {
            result = Mix_PlayMusic(music, loops);
        }

        if (!result)
        {
            spdlog::error("AudioPlayer: Failed to play music'{}': {}", music_path, SDL_GetError());
        }
        else
        {
            spdlog::trace("AudioPlayer: Playing music '{}'。", music_path);
        }
        return result;
    }

    void AudioPlayer::stopMusic(int fade_out_ms)
    {
        if (fade_out_ms > 0)
        {
            Mix_FadeOutMusic(fade_out_ms);
        }
        else
        {
            Mix_HaltMusic();
        }
        spdlog::trace("AudioPlayer: Music stopped");
    }

    void AudioPlayer::pauseMusic()
    {
        Mix_PauseMusic();
        spdlog::trace("AudioPlayer: Music paused");
    }

    void AudioPlayer::resumeMusic()
    {
        Mix_ResumeMusic();
        spdlog::trace("AudioPlayer: Music resumed");
    }

    void AudioPlayer::setChunkVolume(float volume, int channel)
    {
        // float(0-1) to interger(0-128)
        int sdl_volume = static_cast<int>(glm::max(0.0f, glm::min(1.0f, volume)) * MIX_MAX_VOLUME);
        Mix_Volume(channel, sdl_volume);
        spdlog::trace("AudioPlayer: set channel {} volume to {:.2f}。", channel, volume);
    }

    void AudioPlayer::setMusicVolume(float volume)
    {
        int sdl_volume = static_cast<int>(glm::max(0.0f, glm::min(1.0f, volume)) * MIX_MAX_VOLUME);
        Mix_VolumeMusic(sdl_volume);
        spdlog::trace("AudioPlayer: set music volume to {:.2f}。", volume);
    }

    float AudioPlayer::getMusicVolume()
    {
        // interger(0-128) to float(0-1)
        return static_cast<float>(Mix_VolumeMusic(-1)) / static_cast<float>(MIX_MAX_VOLUME);
        /* -1 means quary the volume */
    }

    float AudioPlayer::getChunkVolume(int channel)
    {
        return static_cast<float>(Mix_Volume(channel, -1)) / static_cast<float>(MIX_MAX_VOLUME);
    }
}