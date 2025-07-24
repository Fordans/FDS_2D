#ifndef FDS_TEXTURE_MANAGER_H
#define FDS_TEXTURE_MANAGER_H

#include "SDL3/SDL_render.h"
#include "glm/glm.hpp"

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>

struct SDL_Texture;
struct SDL_Renderer;

namespace fds
{
    class TextureManager final
    {
        friend class ResourceManager;
    public:
        explicit TextureManager(SDL_Renderer* renderer);
        ~TextureManager() = default;

        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager(TextureManager&&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;
    private:
        // Custom deleter for SDL_Texture to ensure proper cleanup
        struct TextureDeleter
        {
            void operator()(SDL_Texture* texture) const noexcept
            {
                if (texture != nullptr)
                {
                    SDL_DestroyTexture(texture);
                }
            }
        };

        SDL_Texture* loadTexture(std::string_view filePath);
        SDL_Texture* getTexture(std::string_view filePath);
        void unloadTexture(std::string_view filePath);
        glm::vec2 getTextureSize(std::string_view filePath) noexcept;
        void clear();
    private:
        SDL_Renderer* renderer_; // Renderer of the game
        std::unordered_map<std::string, std::unique_ptr<SDL_Texture, TextureDeleter>> textureTable_;
    };
}

#endif // FDS_TEXTURE_MANAGER_H