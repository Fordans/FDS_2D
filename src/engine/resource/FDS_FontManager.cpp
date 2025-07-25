#include "FDS_FontManager.h"
#include "spdlog/spdlog.h"

#include <stdexcept>

fds::FontManager::FontManager()
{
    if (!TTF_WasInit() && !TTF_Init())
    {
        throw std::runtime_error("[SDL_ttf]: Failed to init SDL_ttf: " + std::string(SDL_GetError()));
    }
}

fds::FontManager::~FontManager()
{
    if (!fontTable_.empty())
    {
        clear();
    }

    TTF_Quit();
}

TTF_Font *fds::FontManager::loadFont(std::string_view filePath, int fontSize)
{
    if (fontSize <= 0)
    {
        spdlog::error("Font size must be greater than 0. Provided: {}", fontSize);
        return nullptr;
    }

    FontKey key = {std::string(filePath), fontSize};

    auto it = fontTable_.find(key);
    if (it != fontTable_.end())
    {
        return it->second.get();
    }

    TTF_Font* font = TTF_OpenFont(filePath.data(), static_cast<float>(fontSize));
    if (!font)
    {
        spdlog::error("Failed to load font from {}: {}", filePath, SDL_GetError());
        return nullptr;
    }

    fontTable_.emplace(key, std::unique_ptr<TTF_Font, FontDeleter>(font));
    return font;
}

TTF_Font *fds::FontManager::getFont(std::string_view filePath, int fontSize)
{
    FontKey key = {std::string(filePath), fontSize};

    auto it = fontTable_.find(key);
    if (it != fontTable_.end())
    {
        return it->second.get();
    }

    spdlog::warn("Font not found in cache, loading: {} at size {}", filePath, fontSize);
    return loadFont(filePath, fontSize);
}

void fds::FontManager::unloadFont(std::string_view filePath, int fontSize)
{
    FontKey key = {std::string(filePath), fontSize};

    auto it = fontTable_.find(key);
    if (it != fontTable_.end())
    {
        fontTable_.erase(it);
    }
    else
    {
        spdlog::warn("Font not found in cache, cannot unload: {} at size {}", filePath, fontSize);
    }
}

void fds::FontManager::clear()
{
    if(!fontTable_.empty())
    {
        fontTable_.clear();
        spdlog::info("{} fonts cleared.", fontTable_.size());
    }
}