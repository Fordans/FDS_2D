#ifndef FDS_COMPONENT_H
#define FDS_COMPONENT_H

namespace fds
{
    class GameObject;
    class Context;

    class Component
    {
        friend class fds::GameObject;

    protected:
        fds::GameObject *owner_ = nullptr;

    public:
        Component() = default;
        virtual ~Component() = default;

        void setOwner(fds::GameObject *owner) { owner_ = owner; }
        fds::GameObject *getOwner() const { return owner_; }

        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;
        Component(Component &&) = delete;
        Component &operator=(Component &&) = delete;

    protected:
        virtual void init() {}
        virtual void clean() {}
        virtual void handleInput(fds::Context &) {}
        virtual void update(float, fds::Context &) = 0;
        virtual void render(fds::Context &) {}
    };
}

#endif // FDS_COMPONENT_H