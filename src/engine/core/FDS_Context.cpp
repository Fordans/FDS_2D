#include "FDS_Context.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/input/FDS_InputManager.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/render/FDS_Camera.h"
#include "engine/audio/FDS_AudioPlayer.h"
#include "engine/core/FDS_GameState.h"
#include "engine/render/FDS_TextRenderer.h"
#include "engine/physics/FDS_PhysicsEngine.h"

namespace fds
{
    Context::Context(
            fds::InputManager &input_manager,
            fds::Renderer &renderer,
            fds::Camera &camera,
            fds::ResourceManager &resource_manager,
            fds::TextRenderer &text_renderer,
            fds::AudioPlayer &audio_player,
            fds::GameState &game_state,
            fds::PhysicsEngine &physics_engine
        )
        : input_manager_(input_manager)
        , resource_manager_(resource_manager)
        , renderer_(renderer)
        , camera_(camera)
        , text_renderer_(text_renderer)
        , audio_player_(audio_player)
        , game_state_(game_state)
        , physics_engine_(physics_engine)
        {}
}