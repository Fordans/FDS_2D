#include "FDS_UIImage.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/render/FDS_Sprite.h"
#include "engine/core/FDS_Context.h"

#include "spdlog/spdlog.h"

namespace fds
{
    UIImage::UIImage(std::string_view texture_id,
                     glm::vec2 position,
                     glm::vec2 size,
                     std::optional<SDL_FRect> source_rect,
                     bool is_flipped)
        : UIElement(std::move(position), std::move(size)),
          sprite_(texture_id, std::move(source_rect), is_flipped)
    {
        if (texture_id.empty())
        {
            spdlog::warn("UIImage: texture_id is empty, an empty texture created");
        }
    }

    void UIImage::render(fds::Context &context)
    {
        if (!visible_ || sprite_.getId().empty())
        {
            return;
        }

        auto position = getScreenPosition();
        if (size_.x == 0.0f && size_.y == 0.0f)
        {
            context.getRenderer().drawUISprite(sprite_, position);
        }
        else
        {
            context.getRenderer().drawUISprite(sprite_, position, size_);
        }

        UIElement::render(context);
    }

}