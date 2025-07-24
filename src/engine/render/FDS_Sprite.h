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
        : id_(id),
          srcRect_(source_rect),
          isFlipped_(is_flipped)
        {}
        // getter and setter
        [[nodiscard]] const std::string& getId() const { return id_; }
        void setId(std::string_view id) { id_ = id; }
        [[nodiscard]] const std::optional<SDL_FRect>& getSourceRect() const { return srcRect_; }
        void setSourceRect(std::optional<SDL_FRect> source_rect) { srcRect_ = std::move(source_rect); }
        [[nodiscard]] bool isFlipped() const { return isFlipped_; }
        void setFlipped(bool flipped) { isFlipped_ = flipped; }
    private:
        std::string id_;
        std::optional<SDL_FRect> srcRect_;
        bool isFlipped_ = false;
    };
}

#endif // FDS_SPRITE_H