#ifndef FDS_UI_IMAGE_H
#define FDS_UI_IMAGE_H

#include "FDS_UIElement.h"
#include "engine/render/FDS_Sprite.h"

#include "SDL3/SDL_rect.h"

#include <string>
#include <string_view>
#include <optional>

namespace fds
{
    class UIImage final : public UIElement
    {
    protected:
        fds::Sprite sprite_;

    public:
        UIImage(std::string_view texture_id,
                glm::vec2 position = {0.0f, 0.0f},
                glm::vec2 size = {0.0f, 0.0f},
                std::optional<SDL_FRect> source_rect = std::nullopt,
                bool is_flipped = false);

        void render(fds::Context &context) override;

        const fds::Sprite &getSprite() const { return sprite_; }
        void setSprite(fds::Sprite sprite) { sprite_ = std::move(sprite); }

        std::string_view getTextureId() const { return sprite_.getId(); }
        void setTextureId(std::string_view texture_id) { sprite_.setId(texture_id); }

        const std::optional<SDL_FRect> &getSourceRect() const { return sprite_.getSourceRect(); }
        void setSourceRect(std::optional<SDL_FRect> source_rect) { sprite_.setSourceRect(std::move(source_rect)); }

        bool isFlipped() const { return sprite_.isFlipped(); }
        void setFlipped(bool flipped) { sprite_.setFlipped(flipped); }
    };

}

#endif // FDS_UI_IMAGE_H