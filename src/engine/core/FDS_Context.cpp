#include "FDS_Context.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/input/FDS_InputManager.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/render/FDS_Camera.h"

namespace fds
{
    Context::Context(
            fds::InputManager &input_manager,
            fds::Renderer &renderer,
            fds::Camera &camera,
            fds::ResourceManager &resource_manager
        )
        : input_manager_(input_manager)
        , resource_manager_(resource_manager)
        , renderer_(renderer)
        , camera_(camera)
        {}
}