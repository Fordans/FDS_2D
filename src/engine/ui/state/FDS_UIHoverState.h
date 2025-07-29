#ifndef FDS_UI_HOVER_STATE_H
#define FDS_UI_HOVER_STATE_H

#include "FDS_UIState.h"

namespace fds
{
    class UIHoverState final : public UIState
    {
        friend class fds::UIInteractive;

    public:
        UIHoverState(fds::UIInteractive *owner) : UIState(owner) {}
        ~UIHoverState() override = default;

    private:
        void enter() override;
        std::unique_ptr<UIState> handleInput(fds::Context &context) override;
    };

}

#endif // FDS_UI_HOVER_STATE_H