#include "FDS_TextureManager.h"

#include "SDL3_image/SDL_image.h"
#include "spdlog/spdlog.h"

#include <stdexcept>

fds::TextureManager::TextureManager(SDL_Renderer* renderer)
{
    renderer_ = renderer;
    if (renderer_ == nullptr)
    {
        spdlog::error("Failed to initialize TextureManager: Renderer is null.");
        throw std::runtime_error("Renderer cannot be null");
    }
}

SDL_Texture *fds::TextureManager::loadTexture(std::string_view filePath)
{
    // Check if the texture is already loaded
    auto it = textureTable_.find(std::string(filePath));
    if (it != textureTable_.end())
    {
        return it->second.get();
    }
    // If not, load the texture
    SDL_Texture* texture = IMG_LoadTexture(renderer_, filePath.data());
    if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
        spdlog::warn("Failed to set scale mode for texture {}: {}", filePath, SDL_GetError());
    }
    if (texture == nullptr)
    {
        spdlog::error("Failed to load texture from {}: {}", filePath, SDL_GetError());
        throw std::runtime_error("Failed to load texture");
    }
    textureTable_.emplace(filePath, std::unique_ptr<SDL_Texture, TextureDeleter>(texture));
    return texture;
}

SDL_Texture* fds::TextureManager::getTexture(std::string_view filePath)
{
    auto it = textureTable_.find(filePath.data());
    if (it != textureTable_.end())
    {
        return it->second.get();
    }
    else
    {
        spdlog::warn("Texture not found in TextureManager: {}, now load", filePath);
        return loadTexture(filePath);
    }
}

void fds::TextureManager::unloadTexture(std::string_view filePath)
{
    auto it = textureTable_.find(filePath.data());
    if (it != textureTable_.end())
    {
        textureTable_.erase(it);
    }
    else
    {
        spdlog::warn("Attempted to unload a texture that was not loaded: {}", filePath);
    }
}

glm::vec2 fds::TextureManager::getTextureSize(std::string_view filePath) noexcept
{
    SDL_Texture* texture = getTexture(filePath);
    if (!texture)
    {
        spdlog::error("Failed to get texture size: Texture not found for path {}", filePath);
        return glm::vec2(0);
    }

    glm::vec2 size;
    if (!SDL_GetTextureSize(texture, &size.x, &size.y))
    {
        spdlog::error("Failed to get texture size for {}: {}", filePath, SDL_GetError());
        return glm::vec2(0);
    }
    return size;
}

void fds::TextureManager::clear()
{
    if(!textureTable_.empty())
    {
        textureTable_.clear();
        spdlog::info("{} textures have been cleared from the TextureManager.", textureTable_.size());
    }
}