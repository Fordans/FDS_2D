#ifndef FDS_AUDIO_PLAYER_H
#define FDS_AUDIO_PLAYER_H

#include <string>
#include <string_view>

struct Mix_Chunk;
struct Mix_Music;

namespace fds
{
    class ResourceManager;

    class AudioPlayer final
    {
    private:
        fds::ResourceManager *resource_manager_;
        std::string current_music_;

    public:
        explicit AudioPlayer(fds::ResourceManager *resource_manager);
        ~AudioPlayer();

        AudioPlayer(const AudioPlayer &) = delete;
        AudioPlayer &operator=(const AudioPlayer &) = delete;
        AudioPlayer(AudioPlayer &&) = delete;
        AudioPlayer &operator=(AudioPlayer &&) = delete;

        int playChunk(std::string_view Chunk_path, int channel = -1);
        bool playMusic(std::string_view music_path, int loops = -1, int fade_in_ms = 0);
        void stopMusic(int fade_out_ms = 0);
        void pauseMusic();
        void resumeMusic();
        void setChunkVolume(float volume, int channel = -1);
        void setMusicVolume(float volume);
        float getMusicVolume();
        float getChunkVolume(int channel = -1);
    };
}

#endif // FDS_AUDIO_PLAYER_H