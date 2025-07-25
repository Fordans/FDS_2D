#include "FDS_GameState.h"

#include "spdlog/spdlog.h"

#include <stdexcept>

namespace fds
{
    GameState::GameState(SDL_Window *window, SDL_Renderer *renderer, State initial_state)
        : window_(window), renderer_(renderer), current_state_(initial_state)
    {
        if (window_ == nullptr || renderer_ == nullptr)
        {
            spdlog::error("Error initializing GameState: window or renderer is null");
            throw std::runtime_error("Error initializing GameState: window or renderer is null");
        }
    }

    void GameState::setState(State new_state)
    {
        if (current_state_ != new_state)
        {
            spdlog::debug("Game State changed");
            current_state_ = std::move(new_state);
        }
        else
        {
            spdlog::debug("Attempted to set game state to the same state it already is");
        }
    }

    glm::vec2 GameState::getWindowSize() const
    {
        int width, height;
        SDL_GetWindowSize(window_, &width, &height);
        return glm::vec2(width, height);
    }

    void GameState::setWindowSize(const glm::vec2 &window_size)
    {
        SDL_SetWindowSize(window_, static_cast<int>(window_size.x), static_cast<int>(window_size.y));
    }

    glm::vec2 GameState::getLogicalSize() const
    {
        int width, height;
        SDL_GetRenderLogicalPresentation(renderer_, &width, &height, NULL);
        return glm::vec2(width, height);
    }

    void GameState::setLogicalSize(const glm::vec2 &logical_size)
    {
        SDL_SetRenderLogicalPresentation(renderer_,
                                         static_cast<int>(logical_size.x),
                                         static_cast<int>(logical_size.y),
                                         SDL_LOGICAL_PRESENTATION_LETTERBOX);
        spdlog::trace("Logical size: {}x{}", logical_size.x, logical_size.y);
    }

}