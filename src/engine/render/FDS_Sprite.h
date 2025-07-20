#ifndef FDS_SPRITE_H
#define FDS_SPRITE_H

#include "SDL3/SDL_rect.h"

#include <optional>
#include <string>
#include <string_view>

namespace fds
{
    class Sprite final
    {
    public:
        Sprite() = default;
        Sprite(std::string_view id, const std::optional<SDL_FRect>& source_rect = std::nullopt, bool is_flipped = false)
        : m_id(id),
          m_srcRect(source_rect),
          m_isFlipped(is_flipped)
        {}
        // getter and setter
        [[nodiscard]] const std::string& getId() const { return m_id; }
        void setId(std::string_view id) { m_id = id; }
        [[nodiscard]] const std::optional<SDL_FRect>& getSourceRect() const { return m_srcRect; }
        void setSourceRect(std::optional<SDL_FRect> source_rect) { m_srcRect = std::move(source_rect); }
        [[nodiscard]] bool isFlipped() const { return m_isFlipped; }
        void setFlipped(bool flipped) { m_isFlipped = flipped; }
    private:
        std::string m_id;
        std::optional<SDL_FRect> m_srcRect;
        bool m_isFlipped = false;
    };
}

#endif // FDS_SPRITE_H