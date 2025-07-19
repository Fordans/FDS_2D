#include "FDS_ResourceManager.h"
#include "FDS_TextureManager.h"
#include "FDS_AudioManager.h"
#include "FDS_FontManager.h"

#include "SDL3/SDL.h"

fds::ResourceManager::ResourceManager(SDL_Renderer* renderer)
{
    m_textureManager = std::make_unique<TextureManager>(renderer);
    m_audioManager = std::make_unique<AudioManager>();
    m_fontManager = std::make_unique<FontManager>();
}

void fds::ResourceManager::clear()
{
    m_textureManager->clear();
    m_audioManager->clear();
    m_fontManager->clear();
}

SDL_Texture* fds::ResourceManager::loadTexture(std::string_view filePath)
{
    return m_textureManager->loadTexture(filePath);
}

SDL_Texture* fds::ResourceManager::getTexture(std::string_view filePath)
{
    return m_textureManager->getTexture(filePath);
}

glm::vec2 fds::ResourceManager::getTextureSize(std::string_view filePath)
{
    return m_textureManager->getTextureSize(filePath);
}

void fds::ResourceManager::unloadTexture(std::string_view filePath)
{
    m_textureManager->unloadTexture(filePath);
}

void fds::ResourceManager::clearTextures()
{
    m_textureManager->clear();
}

Mix_Chunk* fds::ResourceManager::loadChunk(std::string_view filePath)
{
    return m_audioManager->loadChunk(filePath);
}

Mix_Chunk* fds::ResourceManager::getChunk(std::string_view filePath)
{
    return m_audioManager->getChunk(filePath);
}

void fds::ResourceManager::unloadChunk(std::string_view filePath)
{
    m_audioManager->unloadChunk(filePath);
}

void fds::ResourceManager::clearChunks()
{
    m_audioManager->clearChunk();
}

Mix_Music* fds::ResourceManager::loadMusic(std::string_view filePath)
{
    return m_audioManager->loadMusic(filePath);
}

Mix_Music* fds::ResourceManager::getMusic(std::string_view filePath)
{
    return m_audioManager->getMusic(filePath);
}

void fds::ResourceManager::unloadMusic(std::string_view filePath)
{
    m_audioManager->unloadMusic(filePath);
}

void fds::ResourceManager::clearMusic()
{
    m_audioManager->clearMusic();
}

TTF_Font* fds::ResourceManager::loadFont(std::string_view filePath, int fontSize)
{
    return m_fontManager->loadFont(filePath, fontSize);
}

TTF_Font* fds::ResourceManager::getFont(std::string_view filePath, int fontSize)
{
    return m_fontManager->getFont(filePath, fontSize);
}

void fds::ResourceManager::unloadFont(std::string_view filePath, int fontSize)
{
    m_fontManager->unloadFont(filePath, fontSize);
}

void fds::ResourceManager::clearFonts()
{
    m_fontManager->clear();
}