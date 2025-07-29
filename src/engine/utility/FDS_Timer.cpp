#include "FDS_Timer.h"

#include <thread>

namespace fds
{
    void Timer::wait(int milliseconds, std::function<void()> callback)
    {
        std::thread([milliseconds, callback](){
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            if (callback)
            {
                callback();
            } 
        }).detach();
    }
}