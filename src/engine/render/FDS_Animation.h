#ifndef FDS_ANIMATION_H
#define FDS_ANIMATION_H

#include "SDL3/SDL_rect.h"

#include <vector>
#include <string>
#include <string_view>

namespace fds
{
    struct AnimationFrame
    {
        SDL_FRect source_rect;
        float duration;
    };

    class Animation final
    {
    private:
        std::string name_;
        std::vector<AnimationFrame> frames_;
        float total_duration_ = 0.0f;
        bool loop_ = true;

    public:
        Animation(std::string_view name = "default", bool loop = true);
        ~Animation() = default;

        Animation(const Animation &) = delete;
        Animation &operator=(const Animation &) = delete;
        Animation(Animation &&) = delete;
        Animation &operator=(Animation &&) = delete;

        void addFrame(SDL_FRect source_rect, float duration);

        const AnimationFrame &getFrame(float time) const;

        // --- Setters and Getters ---
        std::string_view getName() const { return name_; }                       
        const std::vector<AnimationFrame> &getFrames() const { return frames_; } 
        size_t getFrameCount() const { return frames_.size(); }                  
        float getTotalDuration() const { return total_duration_; }               
        bool isLooping() const { return loop_; }
        bool isEmpty() const { return frames_.empty(); }                         

        void setName(std::string_view name) { name_ = name; }
        void setLooping(bool loop) { loop_ = loop; }
    };
}

#endif // FDS_ANIMATION_H