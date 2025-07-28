#include "FDS_UIInteractive.h"
#include "engine/ui/state/FDS_UIState.h"
#include "engine/core/FDS_Context.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/audio/FDS_AudioPlayer.h"

#include "spdlog/spdlog.h"

namespace fds
{

    UIInteractive::~UIInteractive() = default;

    UIInteractive::UIInteractive(fds::Context &context, glm::vec2 position, glm::vec2 size)
        : UIElement(std::move(position), std::move(size)), context_(context)
    {}

    void UIInteractive::setState(std::unique_ptr<fds::UIState> state)
    {
        if (!state)
        {
            spdlog::warn("Attempted to set null state on UIInteractive");
            return;
        }

        state_ = std::move(state);
        state_->enter();
    }

    void UIInteractive::addSprite(std::string_view name, std::unique_ptr<fds::Sprite> sprite)
    {
        if (size_.x == 0.0f && size_.y == 0.0f)
        {
            size_ = context_.getResourceManager().getTextureSize(sprite->getId());
        }
        sprites_[std::string(name)] = std::move(sprite);
    }

    void UIInteractive::setSprite(std::string_view name)
    {
        if (sprites_.find(std::string(name)) != sprites_.end())
        {
            current_sprite_ = sprites_[std::string(name)].get();
        }
        else
        {
            spdlog::warn("Sprite '{}' not found", name);
        }
    }

    void UIInteractive::addChunk(std::string_view name, std::string_view path)
    {
        Chunks_[std::string(name)] = path;
    }

    void UIInteractive::playChunk(std::string_view name)
    {
        if (Chunks_.find(std::string(name)) != Chunks_.end())
        {
            context_.getAudioPlayer().playChunk(Chunks_[std::string(name)]);
        }
        else
        {
            spdlog::error("Chunk '{}' not found", name);
        }
    }

    bool UIInteractive::handleInput(fds::Context &context)
    {
        if (UIElement::handleInput(context))
        {
            return true;
        }

        if (state_ && interactive_)
        {
            if (auto next_state = state_->handleInput(context); next_state)
            {
                setState(std::move(next_state));
                return true;
            }
        }
        return false;
    }

    void UIInteractive::render(fds::Context &context)
    {
        if (!visible_)
            return;

        context.getRenderer().drawUISprite(*current_sprite_, getScreenPosition(), size_);

        UIElement::render(context);
    }

}