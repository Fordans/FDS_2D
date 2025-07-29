#ifndef FDS_UI_NORMAL_STATE_H
#define FDS_UI_NORMAL_STATE_H
#include "FDS_UIState.h"

namespace fds
{
    class UINormalState final : public UIState
    {
        friend class fds::UIInteractive;

    public:
        UINormalState(fds::UIInteractive *owner) : UIState(owner) {}
        ~UINormalState() override = default;

    private:
        void enter() override;
        std::unique_ptr<UIState> handleInput(fds::Context &context) override;
    };

}

#endif // FDS_UI_NORMAL_STATE_H