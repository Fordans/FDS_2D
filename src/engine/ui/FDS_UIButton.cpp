#include "FDS_UIButton.h"
#include "engine/ui/state/FDS_UINormalState.h"

#include "spdlog/spdlog.h"

namespace fds
{
    UIButton::UIButton(fds::Context &context,
                       std::string_view normal_sprite_id,
                       std::string_view hover_sprite_id,
                       std::string_view pressed_sprite_id,
                       std::string_view button_hover_chunk_id,
                       std::string_view button_pressed_chunk_id,
                       glm::vec2 position,
                       glm::vec2 size,
                       std::function<void()> callback)
        : UIInteractive(context, std::move(position), std::move(size)), callback_(std::move(callback))
    {
        addSprite("normal", std::make_unique<fds::Sprite>(normal_sprite_id));
        addSprite("hover", std::make_unique<fds::Sprite>(hover_sprite_id));
        addSprite("pressed", std::make_unique<fds::Sprite>(pressed_sprite_id));

        setState(std::make_unique<fds::UINormalState>(this));

        addChunk("hover", button_hover_chunk_id);
        addChunk("pressed", button_pressed_chunk_id);
    }

    void UIButton::clicked()
    {
        if (callback_)
            callback_();
    }
}