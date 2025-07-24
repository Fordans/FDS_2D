#include "FDS_Animation.h"

#include "glm/common.hpp"
#include "spdlog/spdlog.h"

namespace fds
{
    Animation::Animation(std::string_view name, bool loop)
        : name_(name), loop_(loop) {}

    void Animation::addFrame(SDL_FRect source_rect, float duration)
    {
        if (duration <= 0.0f)
        {
            spdlog::warn("Attempted to add frame with duration <= 0.0f to animation '{}", name_);
            return;
        }
        frames_.push_back({source_rect, duration});
        total_duration_ += duration;
    }

    const AnimationFrame &Animation::getFrame(float time) const
    {
        if (frames_.empty())
        {
            spdlog::error("Failed to get frame from animation '{}', animation has no frames", name_);
            return frames_.back();
        }

        float current_time = time;

        if (loop_ && total_duration_ > 0.0f)
        {
            current_time = glm::mod(time, total_duration_);
        }
        else
        {
            if (current_time >= total_duration_)
            {
                return frames_.back();
            }
        }

        float accumulated_time = 0.0f;
        for (const auto &frame : frames_)
        {
            accumulated_time += frame.duration;
            if (current_time < accumulated_time)
            {
                return frame;
            }
        }

        spdlog::warn("Exception occured in Animation::getFrame {}, returning last frame", name_);
        return frames_.back();
    }
}