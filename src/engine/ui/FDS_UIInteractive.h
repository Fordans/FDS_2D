#ifndef FDS_UI_INTERACTIVE_H
#define FDS_UI_INTERACTIVE_H

#include "FDS_UIElement.h"
#include "engine/ui/state/FDS_UIState.h"
#include "engine/render/FDS_Sprite.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace fds
{
    class Context;
}

namespace fds
{
    class UIInteractive : public UIElement
    {
    protected:
        fds::Context &context_;
        std::unique_ptr<fds::UIState> state_;
        std::unordered_map<std::string, std::unique_ptr<fds::Sprite>> sprites_;
        std::unordered_map<std::string, std::string> Chunks_;
        fds::Sprite *current_sprite_ = nullptr;
        bool interactive_ = true;

    public:
        UIInteractive(fds::Context &context, glm::vec2 position = {0.0f, 0.0f}, glm::vec2 size = {0.0f, 0.0f});
        ~UIInteractive() override;

        virtual void clicked() {}

        void addSprite(std::string_view name, std::unique_ptr<fds::Sprite> sprite);
        void setSprite(std::string_view name);
        void addChunk(std::string_view name, std::string_view path);
        void playChunk(std::string_view name);

        void setState(std::unique_ptr<fds::UIState> state);
        fds::UIState *getState() const { return state_.get(); }
        void setInteractive(bool interactive) { interactive_ = interactive; }
        bool isInteractive() const { return interactive_; }

        bool handleInput(fds::Context &context) override;
        void render(fds::Context &context) override;
    };

}

#endif // FDS_UI_INTERACTIVE_H