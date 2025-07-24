#include "FDS_Time.h"

#include "SDL3/SDL_timer.h"
#include "spdlog/spdlog.h"

fds::Time::Time()
{
    lastFrameTime_ = SDL_GetTicksNS();
    frameStartTime_ = lastFrameTime_;
}
int fds::Time::getTargetFPS() const noexcept
{
    return targetFPS_;
}
double fds::Time::getTimeScale() const noexcept
{
    return timeScale_;
}
void fds::Time::setTargetFPS(int fps) noexcept
{
    if (fps >= 0)
    {
        targetFPS_ = fps;
        targetFrameTime_ = 1.0 / static_cast<double>(fps);
    }
    else
    {
        spdlog::warn("Attempted to set target FPS to a non-positive value: {}", fps);
    }
}
void fds::Time::setTimeScale(double scale) noexcept
{
    if (scale >= 0.0)
    {
        timeScale_ = scale;
    }
    else
    {
        spdlog::warn("Attempted to set time scale to a non-positive value: {}", scale);
    }
}
void fds::Time::update() noexcept
{
    frameStartTime_ = SDL_GetTicksNS();
    auto currentFrameTime = static_cast<double>(frameStartTime_ - lastFrameTime_) / 1e9; // Convert to seconds
    if (targetFPS_ > 0)
    {
        limitFrameRate(currentFrameTime);
    }
    else
    {
        deltaTime_ = currentFrameTime;
    }
}
float fds::Time::getDeltaTime() const noexcept
{
    return static_cast<float>(deltaTime_ * timeScale_);
}
void fds::Time::limitFrameRate(double currentFrameTime) noexcept
{
    if (currentFrameTime < targetFrameTime_)
    {
        Uint64 delayTime = static_cast<Uint64>((targetFrameTime_ - currentFrameTime) * 1e9);
        SDL_DelayNS(delayTime);
    }
    frameStartTime_ = SDL_GetTicksNS();
    deltaTime_ = static_cast<double>(frameStartTime_ - lastFrameTime_) / 1e9;
    lastFrameTime_ = frameStartTime_;
}