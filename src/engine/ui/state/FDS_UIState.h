#ifndef FDS_UI_STATE_H
#define FDS_UI_STATE_H

#include <memory>

namespace fds
{
    class Context;
    class UIInteractive;

    class UIState
    {
        friend class fds::UIInteractive;

    protected:
        fds::UIInteractive *owner_ = nullptr;

    public:
        UIState(fds::UIInteractive *owner) : owner_(owner) {}
        virtual ~UIState() = default;

        UIState(const UIState &) = delete;
        UIState &operator=(const UIState &) = delete;
        UIState(UIState &&) = delete;
        UIState &operator=(UIState &&) = delete;

    protected:
        virtual void enter() {}
        virtual std::unique_ptr<UIState> handleInput(fds::Context &context) = 0;
    };

}

#endif // FDS_UI_STATE_H