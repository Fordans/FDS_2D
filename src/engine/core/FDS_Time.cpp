#include "FDS_Time.h"

#include "SDL3/SDL_timer.h"
#include "spdlog/spdlog.h"

fds::Time::Time()
{
    m_lastFrameTime = SDL_GetTicksNS();
    m_frameStartTime = m_lastFrameTime;
}

int fds::Time::getTargetFPS() const noexcept
{
    return m_targetFPS;
}

double fds::Time::getTimeScale() const noexcept
{
    return m_timeScale;
}

void fds::Time::setTargetFPS(int fps) noexcept
{
    if (fps >= 0)
    {
        m_targetFPS = fps;
        m_targetFrameTime = 1.0 / static_cast<double>(fps);
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
        m_timeScale = scale;
    }
    else
    {
        spdlog::warn("Attempted to set time scale to a non-positive value: {}", scale);
    }
}

void fds::Time::update() noexcept
{
    m_frameStartTime = SDL_GetTicksNS();
    auto currentFrameTime = static_cast<double>(m_frameStartTime - m_lastFrameTime) / 1e9; // Convert to seconds
    if(m_targetFPS > 0)
    {
        limitFrameRate(currentFrameTime);
    }
    else
    {
        m_deltaTime = currentFrameTime;
    }
}

float fds::Time::getDeltaTime() const noexcept
{
    return static_cast<float>(m_deltaTime * m_timeScale);
}

void fds::Time::limitFrameRate(double currentFrameTime) noexcept
{
    if (currentFrameTime < m_targetFrameTime)
    {
        Uint64 delayTime = static_cast<Uint64>((m_targetFrameTime - currentFrameTime) * 1e9);
        SDL_DelayNS(delayTime);
    }
    m_frameStartTime = SDL_GetTicksNS();
    m_deltaTime = static_cast<double>(m_frameStartTime - m_lastFrameTime) / 1e9;
    m_lastFrameTime = m_frameStartTime;
}
