#include "FDS_GameObject.h"
#include "engine/render/FDS_Camera.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/input/FDS_InputManager.h"

#include "spdlog/spdlog.h"

namespace fds
{
    GameObject::GameObject(std::string_view name, std::string_view tag) : name_(name), tag_(tag)
    {
        spdlog::trace("GameObject created: {} {}", name_, tag_);
    }

    void GameObject::update(float delta_time, fds::Context &context)
    {
        for (auto &pair : components_)
        {
            pair.second->update(delta_time, context);
        }
    }

    void GameObject::render(fds::Context &context)
    {
        for (auto &pair : components_)
        {
            pair.second->render(context);
        }
    }

    void GameObject::clean()
    {
        spdlog::trace("Cleaning GameObject...");
        for (auto &pair : components_)
        {
            pair.second->clean();
        }
        components_.clear();
    }

    void GameObject::handleInput(fds::Context &context)
    {
        for (auto &pair : components_)
        {
            pair.second->handleInput(context);
        }
    }
}