#ifndef FDS_AUDIO_MANAGER_H
#define FDS_AUDIO_MANAGER_H

#include "SDL3_mixer/SDL_mixer.h"

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>

namespace fds
{
    class AudioManager final
    {
        friend class ResourceManager;
    public:
        AudioManager();
        ~AudioManager();

        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;
        AudioManager(AudioManager&&) = delete;
        AudioManager& operator=(AudioManager&&) = delete;
    private:
        struct ChunkDeleter
        {            
            void operator()(Mix_Chunk* chunk) const noexcept
            {
                if (chunk != nullptr)
                {
                    Mix_FreeChunk(chunk);
                }
            }
        };

        struct MusicDeleter
        {
            void operator()(Mix_Music* music) const noexcept
            {
                if (music != nullptr)
                {
                    Mix_FreeMusic(music);
                }
            }
        };

        Mix_Chunk* loadChunk(std::string_view filePath);
        Mix_Music* loadMusic(std::string_view filePath);
        Mix_Chunk* getChunk(std::string_view filePath);
        Mix_Music* getMusic(std::string_view filePath);
        void unloadChunk(std::string_view filePath);
        void unloadMusic(std::string_view filePath);
        void clearChunk();
        void clearMusic();
        void clear();
    private:
        std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, ChunkDeleter>> m_chunkTable;
        std::unordered_map<std::string, std::unique_ptr<Mix_Music, MusicDeleter>> m_musicTable;
    };
}

#endif // FDS_AUDIO_MANAGER_H