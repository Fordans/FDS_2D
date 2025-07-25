#ifndef FDS_GAME_H
#define FDS_GAME_H

#include "FDS_Time.h"

#include "glm/glm.hpp"

#include <memory>
#include <functional>

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
    class SceneManager;
    class AudioPlayer;
    class GameState;
    class TextRenderer;

    class Game final
    {
    public:
        Game();
        ~Game();
        void run();
        void shutdown() noexcept;
        void registerSceneSetup(std::function<void(fds::SceneManager&)> func);

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
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        bool isRunning_;
        std::unique_ptr<fds::Time> time_;
        std::unique_ptr<fds::Config> config_;
        std::unique_ptr<fds::ResourceManager> resourceManager_;
        std::unique_ptr<fds::Renderer> fdsRenderer_;
        std::unique_ptr<fds::InputManager> inputManager_;
        std::unique_ptr<fds::Camera> camera_;
        std::unique_ptr<fds::Context> context_;
        std::unique_ptr<fds::SceneManager> sceneManager_;
        std::unique_ptr<fds::AudioPlayer> audioPlayer_;
        std::unique_ptr<fds::GameState> gameState_;
        std::unique_ptr<fds::TextRenderer> textRenderer_;

        std::function<void(fds::SceneManager&)> scene_setup_func_;
    };
}

#endif // FDS_GAME_H