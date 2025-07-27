#ifndef FDS_UI_PANEL_H
#define FDS_UI_PANEL_H

#include "engine/utility/FDS_math.h"
#include "FDS_UIElement.h"

#include <optional>

namespace fds
{

    class UIPanel final : public UIElement
    {
        std::optional<fds::FColor> background_color_;

    public:
        explicit UIPanel(glm::vec2 position = {0.0f, 0.0f}, glm::vec2 size = {0.0f, 0.0f},
                         std::optional<fds::FColor> background_color = std::nullopt);

        void setBackgroundColor(std::optional<fds::FColor> background_color) { background_color_ = std::move(background_color); }
        const std::optional<fds::FColor> &getBackgroundColor() const { return background_color_; }

        void render(fds::Context &context) override;
    };

}

#endif //FDS_UI_PANEL_H