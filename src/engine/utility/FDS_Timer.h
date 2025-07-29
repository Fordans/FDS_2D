#ifndef FDS_TIMER_H
#define FDS_TIMER_H

#include <chrono>
#include <functional>

namespace fds
{
    class Timer
    {
    public:
        Timer() = default;
        ~Timer() = default;

        void wait(int milliseconds, std::function<void()> callback);

        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;
        Timer(Timer &&) = delete;
        Timer &operator=(Timer &&) = delete;
    };
}

#endif // FDS_TIMER_H