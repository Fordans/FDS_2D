#ifndef FDS_RESOURCE_MANAGER_H
#define FDS_RESOURCE_MANAGER_H

#include "FDS_ResourceManager.h"
#include "FDS_TextureManager.h"
#include "FDS_AudioManager.h"
#include "FDS_FontManager.h"

#include "glm/glm.hpp"

#include <memory>
#include <string>
#include <string_view>

struct SDL_Renderer;
struct SDL_Texture;
struct Mix_Chunk;
struct Mix_Music;
struct TTF_Font;

namespace fds
{
    class ResourceManager final
    {
    public:
        explicit ResourceManager(SDL_Renderer* renderer);
        ~ResourceManager() = default;
        // Texture management
        SDL_Texture* loadTexture(std::string_view filePath);
        SDL_Texture* getTexture(std::string_view filePath);
        glm::vec2 getTextureSize(std::string_view filePath);
        void unloadTexture(std::string_view filePath);
        void clearTextures();
        // Audio management
        Mix_Chunk* loadChunk(std::string_view filePath);
        Mix_Chunk* getChunk(std::string_view filePath);
        void unloadChunk(std::string_view filePath);
        void clearChunks();
        Mix_Music* loadMusic(std::string_view filePath);
        Mix_Music* getMusic(std::string_view filePath);
        void unloadMusic(std::string_view filePath);
        void clearMusic();
        // Font management
        TTF_Font* loadFont(std::string_view filePath, int fontSize);
        TTF_Font* getFont(std::string_view filePath, int fontSize);
        void unloadFont(std::string_view filePath, int fontSize);
        void clearFonts();

        void clear();

        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
    private:
        std::unique_ptr<TextureManager> m_textureManager;
        std::unique_ptr<AudioManager> m_audioManager;
        std::unique_ptr<FontManager> m_fontManager;
    };
}
#endif // FDS_RESOURCE_MANAGER_H