#ifndef FDS_CONTEXT_H
#define FDS_CONTEXT_H

#include "engine/utility/FDS_fwd.h"

namespace fds
{
    class Context final
    {
    private:
        fds::InputManager &input_manager_;
        fds::Renderer &renderer_;
        fds::Camera &camera_;
        fds::ResourceManager &resource_manager_;
        fds::TextRenderer &text_renderer_;
        fds::AudioPlayer &audio_player_;
        fds::GameState &game_state_;
        fds::PhysicsEngine &physics_engine_;
    public:
        Context(fds::InputManager &input_manager,
                fds::Renderer &renderer,
                fds::Camera &camera,
                fds::ResourceManager &resource_manager,
                fds::TextRenderer &text_renderer,
                fds::AudioPlayer &audio_player,
                fds::GameState &game_state,
                fds::PhysicsEngine &physics_engine
        );

        Context(const Context &) = delete;
        Context &operator=(const Context &) = delete;
        Context(Context &&) = delete;
        Context &operator=(Context &&) = delete;

        fds::InputManager &getInputManager() const { return input_manager_; }
        fds::Renderer &getRenderer() const { return renderer_; }
        fds::Camera &getCamera() const { return camera_; }
        fds::ResourceManager &getResourceManager() const { return resource_manager_; }
        fds::TextRenderer &getTextRenderer() const { return text_renderer_; }
        fds::AudioPlayer &getAudioPlayer() const { return audio_player_; }
        fds::GameState &getGameState() const { return game_state_; }
        fds::PhysicsEngine &getPhysicsEngine() const { return physics_engine_; }
    };
}

#endif // FDS_CONTEXT_H