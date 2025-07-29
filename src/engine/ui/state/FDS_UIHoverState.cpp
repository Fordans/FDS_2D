#include "FDS_UIHoverState.h"
#include "FDS_UINormalState.h"
#include "FDS_UIPressedState.h"
#include "engine/ui/FDS_UIInteractive.h"
#include "engine/input/FDS_InputManager.h"
#include "engine/core/FDS_Context.h"

#include "spdlog/spdlog.h"

namespace fds
{

    void UIHoverState::enter()
    {
        owner_->setSprite("hover");
        spdlog::debug("switch to hover state");
    }

    std::unique_ptr<UIState> UIHoverState::handleInput(fds::Context &context)
    {
        auto &input_manager = context.getInputManager();
        auto mouse_pos = input_manager.getLogicalMousePosition();
        if (!owner_->isPointInside(mouse_pos))
        {
            return std::make_unique<UINormalState>(owner_);
        }
        if (input_manager.isActionPressed("MouseLeftClick"))
        {
            return std::make_unique<UIPressedState>(owner_);
        }
        return nullptr;
    }

}