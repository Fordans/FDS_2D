#include "FDS_UIPanel.h"
#include "engine/core/FDS_Context.h"
#include "engine/render/FDS_Renderer.h"

#include "SDL3/SDL_pixels.h"
#include "spdlog/spdlog.h"

namespace fds
{
    UIPanel::UIPanel(glm::vec2 position, glm::vec2 size, std::optional<fds::FColor> background_color)
        : UIElement(std::move(position), std::move(size)), background_color_(std::move(background_color))
    {}

    void UIPanel::render(fds::Context &context)
    {
        if (!visible_)
            return;

        if (background_color_)
        {
            context.getRenderer().drawUIFilledRect(getBounds(), background_color_.value());
        }

        UIElement::render(context);
    }

}