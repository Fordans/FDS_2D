#ifndef FDS_GAME_H
#define FDS_GAME_H

#include "FDS_Time.h"

#include "glm/glm.hpp"

#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace fds
{
    class ResourceManager;
    class Renderer;
    class Config;
    class InputManager;
    class Context;
    class Camera;

    class Game final
    {
    public:
        Game();
        ~Game();
        void run();
        void shutdown() noexcept;

        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;
        Game(Game&&) = delete;
        Game& operator=(Game&&) = delete;
    private:
        [[nodiscard]] bool init();
        void clean();
        void handleEvents();
        void update(float deltaTime);
        void render();
    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        bool m_isRunning;
        std::unique_ptr<fds::Time> m_time;
        std::unique_ptr<fds::Config> m_config;
        std::unique_ptr<fds::ResourceManager> m_resourceManager;
        std::unique_ptr<fds::Renderer> m_fdsRenderer;
        std::unique_ptr<fds::InputManager> m_inputManager;
        std::unique_ptr<fds::Camera> m_camera;
        std::unique_ptr<fds::Context> m_context;
    };
}

#endif // FDS_GAME_H