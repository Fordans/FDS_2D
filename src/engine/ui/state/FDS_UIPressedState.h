#ifndef FDS_UI_PRESSED_STATE_H
#define FDS_UI_PRESSED_STATE_H

#include "FDS_UIState.h"

namespace fds
{
    class UIPressedState final : public UIState
    {
        friend class fds::UIInteractive;

    public:
        UIPressedState(fds::UIInteractive *owner) : UIState(owner) {}
        ~UIPressedState() override = default;

    private:
        void enter() override;
        std::unique_ptr<UIState> handleInput(fds::Context &context) override;
    };

}

#endif // FDS_UI_PRESSED_STATE_H