#ifndef FDS_UI_BUTTON_H
#define FDS_UI_BUTTON_H

#include "FDS_UIInteractive.h"

#include <functional>
#include <utility>

namespace fds
{
    class UIButton final : public UIInteractive
    {
    private:
        std::function<void()> callback_;

    public:
        UIButton(fds::Context &context,
                 std::string_view normal_sprite_id,
                 std::string_view hover_sprite_id,
                 std::string_view pressed_sprite_id,
                 std::string_view button_hover_chunk_id,
                 std::string_view button_pressed_chunk_id,
                 glm::vec2 position = {0.0f, 0.0f},
                 glm::vec2 size = {0.0f, 0.0f},
                 std::function<void()> callback = nullptr);
        ~UIButton() override = default;

        void clicked() override;

        void setCallback(std::function<void()> callback) { callback_ = std::move(callback); }
        std::function<void()> getCallback() const { return callback_; }
    };
}

#endif // FDS_UI_BUTTON_H