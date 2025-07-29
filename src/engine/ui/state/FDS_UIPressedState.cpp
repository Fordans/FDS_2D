#include "FDS_UIHoverState.h"
#include "FDS_UINormalState.h"
#include "FDS_UIPressedState.h"
#include "engine/ui/FDS_UIInteractive.h"
#include "engine/input/FDS_InputManager.h"
#include "engine/core/FDS_Context.h"

#include "spdlog/spdlog.h"

namespace fds
{
    void UIPressedState::enter()
    {
        owner_->setSprite("pressed");
        owner_->playChunk("pressed");
        spdlog::debug("switch to pressed state");
    }

    std::unique_ptr<UIState> UIPressedState::handleInput(fds::Context &context)
    {
        auto &input_manager = context.getInputManager();
        auto mouse_pos = input_manager.getLogicalMousePosition();
        if (input_manager.isActionReleased("MouseLeftClick"))
        {
            if (!owner_->isPointInside(mouse_pos))
            {
                return std::make_unique<fds::UINormalState>(owner_);
            }
            else
            {
                owner_->clicked();
                return std::make_unique<fds::UIHoverState>(owner_);
            }
        }

        return nullptr;
    }
}