#ifndef FDS_GAME_STATE_H
#define FDS_GAME_STATE_H

#include "glm/vec2.hpp"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"

#include <optional>
#include <string>

namespace fds
{
    enum class State
    {
        Title,
        Playing,
        Paused,
        GameOver,
        // more State ...
    };

    class GameState final
    {
    private:
        SDL_Window *window_ = nullptr;
        SDL_Renderer *renderer_ = nullptr;
        State current_state_ = State::Title;

    public:
        explicit GameState(SDL_Window *window, SDL_Renderer *renderer, State initial_state = State::Title);

        State getCurrentState() const { return current_state_; }
        void setState(State new_state);
        glm::vec2 getWindowSize() const;
        void setWindowSize(const glm::vec2 &window_size);
        glm::vec2 getLogicalSize() const;
        void setLogicalSize(const glm::vec2 &logical_size);

        bool isInTitle() const { return current_state_ == State::Title; }
        bool isPlaying() const { return current_state_ == State::Playing; }
        bool isPaused() const { return current_state_ == State::Paused; }
        bool isGameOver() const { return current_state_ == State::GameOver; }
    };

}
#endif // FDS_GAMESTATE_H