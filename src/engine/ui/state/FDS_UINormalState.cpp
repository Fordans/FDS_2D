#include "FDS_UINormalState.h"
#include "FDS_UIHoverState.h"
#include "engine/ui/FDS_UIInteractive.h"
#include "engine/input/FDS_InputManager.h"
#include "engine/core/FDS_Context.h"
#include "engine/audio/FDS_AudioPlayer.h"

#include "spdlog/spdlog.h"

namespace fds
{
    void UINormalState::enter()
    {
        owner_->setSprite("normal");
        spdlog::debug("switch to normal state");
    }

    std::unique_ptr<UIState> UINormalState::handleInput(fds::Context &context)
    {
        auto &input_manager = context.getInputManager();
        auto mouse_pos = input_manager.getLogicalMousePosition();
        if (owner_->isPointInside(mouse_pos))
        {
            owner_->playChunk("hover");
            return std::make_unique<fds::UIHoverState>(owner_);
        }
        return nullptr;
    }
}