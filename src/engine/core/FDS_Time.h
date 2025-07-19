#ifndef FDS_TIME_H
#define FDS_TIME_H

#include "SDL3/SDL_stdinc.h"

namespace fds
{
    class Time final
    {
    public:
        Time();
        ~Time() = default;

        int getTargetFPS() const noexcept;
        double getTimeScale() const noexcept;
        void setTargetFPS(int fps) noexcept;
        void setTimeScale(double scale) noexcept;

        void update() noexcept;
        float getDeltaTime() const noexcept;

        Time(const Time&) = delete;
        Time& operator=(const Time&) = delete;
        Time(Time&&) = delete;
        Time& operator=(Time&&) = delete;
    private:
        void limitFrameRate(double current_time) noexcept;
    private:
        int m_targetFPS = 0; // Default target FPS (0 means no limit)
        double m_timeScale = 1.0; // Default time scale (1.0)
        double m_targetFrameTime = 0.0;
        double m_deltaTime = 0.0;
        Uint64 m_lastFrameTime;
        Uint64 m_frameStartTime;
    };
}

#endif // FDS_TIME_H