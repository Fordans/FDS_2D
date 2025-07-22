#ifndef FDS_CONTEXT_H
#define FDS_CONTEXT_H

namespace fds
{
    class InputManager;
    class Renderer;
    class Camera;
    class ResourceManager;

    class Context final
    {
    private:
        fds::InputManager &input_manager_;
        fds::Renderer &renderer_;
        fds::Camera &camera_;
        fds::ResourceManager &resource_manager_;

    public:
        Context(fds::InputManager &input_manager,
                fds::Renderer &renderer,
                fds::Camera &camera,
                fds::ResourceManager &resource_manager
        );

        Context(const Context &) = delete;
        Context &operator=(const Context &) = delete;
        Context(Context &&) = delete;
        Context &operator=(Context &&) = delete;

        fds::InputManager &getInputManager() const { return input_manager_; }
        fds::Renderer &getRenderer() const { return renderer_; }
        fds::Camera &getCamera() const { return camera_; }
        fds::ResourceManager &getResourceManager() const { return resource_manager_; }
    };
}

#endif // FDS_CONTEXT_H