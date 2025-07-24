#ifndef FDS_FONT_MANAGER_H
#define FDS_FONT_MANAGER_H

#include "SDL3_ttf/SDL_ttf.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <functional>

namespace fds
{
    using FontKey = std::pair<std::string, int>;

    struct FontKeyHash
    {
        std::size_t operator()(const FontKey& key) const
        {
        std::hash<std::string> string_hasher;
        std::hash<int> int_hasher;
        return string_hasher(key.first) ^ int_hasher(key.second); // Combine the two hash values
        }
    };

    class FontManager final
    {
        friend class ResourceManager;
    public:
        FontManager();
        ~FontManager();

        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;
        FontManager(FontManager&&) = delete;   
        FontManager& operator=(FontManager&&) = delete;
    private:
        struct FontDeleter
        {
            void operator()(TTF_Font* font) const
            {
                if (font)
                {
                    TTF_CloseFont(font);
                }
            }
        };

        TTF_Font* loadFont(std::string_view filePath, int fontSize);
        TTF_Font* getFont(std::string_view filePath, int fontSize);
        void unloadFont(std::string_view filePath, int fontSize);
        void clear();
    private:
        std::unordered_map<FontKey, std::unique_ptr<TTF_Font, FontDeleter>, FontKeyHash> fontTable_;
    };
}

#endif // FDS_FONT_MANAGER_H