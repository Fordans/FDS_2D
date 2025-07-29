#include "FDS_UILabel.h"
#include "engine/core/FDS_Context.h"

namespace fds
{
    UILabel::UILabel(fds::TextRenderer &text_renderer,
                     std::string_view text,
                     std::string_view font_id,
                     int font_size,
                     fds::FColor text_color,
                     glm::vec2 position)
        : UIElement(std::move(position)),
          text_renderer_(text_renderer),
          text_(text),
          font_id_(font_id),
          font_size_(font_size),
          text_fcolor_(std::move(text_color))
    {
        size_ = text_renderer_.getTextSize(text_, font_id_, font_size_);
    }

    void UILabel::render(fds::Context &context)
    {
        if (!visible_ || text_.empty())
            return;

        text_renderer_.drawUIText(text_, font_id_, font_size_, getScreenPosition(), text_fcolor_);

        UIElement::render(context);
    }

    void UILabel::setText(std::string_view text)
    {
        text_ = text;
        size_ = text_renderer_.getTextSize(text_, font_id_, font_size_);
    }

    void UILabel::setFontId(std::string_view font_id)
    {
        font_id_ = font_id;
        size_ = text_renderer_.getTextSize(text_, font_id_, font_size_);
    }

    void UILabel::setFontSize(int font_size)
    {
        font_size_ = font_size;
        size_ = text_renderer_.getTextSize(text_, font_id_, font_size_);
    }

    void UILabel::setTextFColor(fds::FColor text_fcolor)
    {
        text_fcolor_ = std::move(text_fcolor);
    }

}