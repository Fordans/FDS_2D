#include "FDS_ResourceManager.h"
#include "FDS_TextureManager.h"
#include "FDS_AudioManager.h"
#include "FDS_FontManager.h"

#include "SDL3/SDL.h"

fds::ResourceManager::ResourceManager(SDL_Renderer* renderer)
{
    textureManager_ = std::make_unique<TextureManager>(renderer);
    audioManager_ = std::make_unique<AudioManager>();
    fontManager_ = std::make_unique<FontManager>();
}

void fds::ResourceManager::clear()
{
    textureManager_->clear();
    audioManager_->clear();
    fontManager_->clear();
}

SDL_Texture* fds::ResourceManager::loadTexture(std::string_view filePath)
{
    return textureManager_->loadTexture(filePath);
}

SDL_Texture* fds::ResourceManager::getTexture(std::string_view filePath)
{
    return textureManager_->getTexture(filePath);
}

glm::vec2 fds::ResourceManager::getTextureSize(std::string_view filePath)
{
    return textureManager_->getTextureSize(filePath);
}

void fds::ResourceManager::unloadTexture(std::string_view filePath)
{
    textureManager_->unloadTexture(filePath);
}

void fds::ResourceManager::clearTextures()
{
    textureManager_->clear();
}

Mix_Chunk* fds::ResourceManager::loadChunk(std::string_view filePath)
{
    return audioManager_->loadChunk(filePath);
}

Mix_Chunk* fds::ResourceManager::getChunk(std::string_view filePath)
{
    return audioManager_->getChunk(filePath);
}

void fds::ResourceManager::unloadChunk(std::string_view filePath)
{
    audioManager_->unloadChunk(filePath);
}

void fds::ResourceManager::clearChunks()
{
    audioManager_->clearChunk();
}

Mix_Music* fds::ResourceManager::loadMusic(std::string_view filePath)
{
    return audioManager_->loadMusic(filePath);
}

Mix_Music* fds::ResourceManager::getMusic(std::string_view filePath)
{
    return audioManager_->getMusic(filePath);
}

void fds::ResourceManager::unloadMusic(std::string_view filePath)
{
    audioManager_->unloadMusic(filePath);
}

void fds::ResourceManager::clearMusic()
{
    audioManager_->clearMusic();
}

TTF_Font* fds::ResourceManager::loadFont(std::string_view filePath, int fontSize)
{
    return fontManager_->loadFont(filePath, fontSize);
}

TTF_Font* fds::ResourceManager::getFont(std::string_view filePath, int fontSize)
{
    return fontManager_->getFont(filePath, fontSize);
}

void fds::ResourceManager::unloadFont(std::string_view filePath, int fontSize)
{
    fontManager_->unloadFont(filePath, fontSize);
}

void fds::ResourceManager::clearFonts()
{
    fontManager_->clear();
}