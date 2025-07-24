#include "FDS_AudioManager.h"

#include "spdlog/spdlog.h"

#include <stdexcept>

fds::AudioManager::AudioManager()
{
    MIX_InitFlags flags = MIX_INIT_MP3;
    if ((Mix_Init(flags) & flags) != flags)
    {
        throw std::runtime_error("[SDL_Mixer] Failed to init SDL_Mixer: " + std::string(SDL_GetError()));
    }

    if (!Mix_OpenAudio(0, nullptr))
    {
        Mix_Quit();
        throw std::runtime_error("[SDL_Mixer] Failed to Open Audio: " + std::string(SDL_GetError()));
    }
}

fds::AudioManager::~AudioManager()
{
    // stop all audio channels and music before cleaning up
    Mix_HaltChannel(-1);
    Mix_HaltMusic();

    clear();

    Mix_CloseAudio();
    Mix_Quit();
}

Mix_Chunk* fds::AudioManager::loadChunk(std::string_view filePath)
{
    auto it = chunkTable_.find(std::string(filePath));
    if (it != chunkTable_.end())
    {
        return it->second.get();
    }

    spdlog::debug("load chunk: {}", filePath);
    Mix_Chunk* chunk = Mix_LoadWAV(filePath.data());
    if (!chunk)
    {
        spdlog::error("Failed to load chunk: '{}': {}", filePath, SDL_GetError());
        return nullptr;
    }

    chunkTable_.emplace(filePath, std::unique_ptr<Mix_Chunk, ChunkDeleter>(chunk));
    return chunk;
}

Mix_Music* fds::AudioManager::loadMusic(std::string_view filePath)
{
    auto it = musicTable_.find(std::string(filePath));
    if (it != musicTable_.end())
    {
        return it->second.get();
    }

    spdlog::debug("load music: {}", filePath);
    Mix_Music* music = Mix_LoadMUS(filePath.data());
    if (!music)
    {
        spdlog::error("Failed to load music: '{}': {}", filePath, SDL_GetError());
        return nullptr;
    }

    musicTable_.emplace(filePath, std::unique_ptr<Mix_Music, MusicDeleter>(music));
    return music;
}

Mix_Chunk* fds::AudioManager::getChunk(std::string_view filePath)
{
    auto it = chunkTable_.find(std::string(filePath));
    if (it != chunkTable_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("Chunk not found: {}, now load", filePath);
        return loadChunk(filePath);
    }
}

Mix_Music* fds::AudioManager::getMusic(std::string_view filePath)
{
    auto it = musicTable_.find(std::string(filePath));
    if (it != musicTable_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("Music not found: {}, now load", filePath);
        return loadMusic(filePath);
    }
}

void fds::AudioManager::unloadChunk(std::string_view filePath)
{
    auto it = chunkTable_.find(std::string(filePath));
    if (it != chunkTable_.end())
    {
        chunkTable_.erase(it);
        spdlog::debug("Unloaded chunk: {}", filePath);
    }
    else
    {
        spdlog::warn("Chunk not found for unloading: {}", filePath);
    }
}

void fds::AudioManager::unloadMusic(std::string_view filePath)
{
    auto it = musicTable_.find(std::string(filePath));
    if (it != musicTable_.end())
    {
        musicTable_.erase(it);
        spdlog::debug("Unloaded music: {}", filePath);
    }
    else
    {
        spdlog::warn("Music not found for unloading: {}", filePath);
    }
}

void fds::AudioManager::clearChunk()
{
    if(!chunkTable_.empty())
    {    
        spdlog::debug("Clearing {} loaded chunks", chunkTable_.size());
        chunkTable_.clear();
    }
}

void fds::AudioManager::clearMusic()
{
    if(!musicTable_.empty())
    {
        spdlog::debug("Clearing {} loaded music tracks", musicTable_.size());
        musicTable_.clear();
    }
}

void fds::AudioManager::clear()
{
    clearChunk();
    clearMusic();
}