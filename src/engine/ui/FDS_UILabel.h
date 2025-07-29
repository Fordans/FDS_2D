#ifndef FDS_UI_LABEL_H
#define FDS_UI_LABEL_H

#include "FDS_UIElement.h"
#include "engine/utility/FDS_math.h"
#include "engine/render/FDS_TextRenderer.h"

#include <string>
#include <string_view>

namespace fds
{
    class UILabel final : public UIElement
    {
    private:
        fds::TextRenderer &text_renderer_;

        std::string text_;
        std::string font_id_;
        int font_size_;
        fds::FColor text_fcolor_ = {1.0f, 1.0f, 1.0f, 1.0f};
    public:
        UILabel(fds::TextRenderer &text_renderer,
                std::string_view text,
                std::string_view font_id,
                int font_size = 16,
                fds::FColor text_color = {1.0f, 1.0f, 1.0f, 1.0f},
                glm::vec2 position = {0.0f, 0.0f});

        void render(fds::Context &context) override;

        std::string_view getText() const { return text_; }
        std::string_view getFontId() const { return font_id_; }
        int getFontSize() const { return font_size_; }
        const fds::FColor &getTextFColor() const { return text_fcolor_; }

        void setText(std::string_view text);
        void setFontId(std::string_view font_id);
        void setFontSize(int font_size);
        void setTextFColor(fds::FColor text_fcolor);
    };

}

#endif // FDS_UI_LABEL_H