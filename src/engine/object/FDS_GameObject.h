#ifndef FDS_GAME_OBJECT_H
#define FDS_GAME_OBJECT_H

#include "engine/component/FDS_Component.h"

#include "spdlog/spdlog.h"

#include <string_view>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <utility>

namespace fds
{
    class Context;

    class GameObject final
    {
    public:
        GameObject(std::string_view name = "", std::string_view tag = "");

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = delete;
        GameObject &operator=(GameObject &&) = delete;

        // setters and getters
        void setName(std::string_view name) { name_ = name; }
        std::string_view getName() const { return name_; }
        void setTag(std::string_view tag) { tag_ = tag; }
        std::string_view getTag() const { return tag_; }
        void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }
        bool isNeedRemove() const { return need_remove_; }

        template <typename T, typename... Args>
        T *addComponent(Args &&...args)
        {
            static_assert(std::is_base_of<fds::Component, T>::value, "T must be a child of Component");

            auto type_index = std::type_index(typeid(T));
            if (hasComponent<T>())
            {
                return getComponent<T>();
            }

            auto new_component = std::make_unique<T>(std::forward<Args>(args)...);
            T *ptr = new_component.get();
            new_component->setOwner(this);
            components_[type_index] = std::move(new_component);
            ptr->init();
            spdlog::debug("GameObject::addComponent: {} added component {}", name_, typeid(T).name());
            return ptr;
        }

        template <typename T>
        T *getComponent() const
        {
            static_assert(std::is_base_of<fds::Component, T>::value, "T must be a child of Component");
            auto type_index = std::type_index(typeid(T));
            auto it = components_.find(type_index);
            if (it != components_.end())
            {
                return static_cast<T *>(it->second.get());
            }
            return nullptr;
        }

        template <typename T>
        bool hasComponent() const
        {
            static_assert(std::is_base_of<fds::Component, T>::value, "T must be a child of Component");
            return components_.contains(std::type_index(typeid(T)));
        }

        template <typename T>
        void removeComponent()
        {
            static_assert(std::is_base_of<fds::Component, T>::value, "T must be a child of Component");
            auto type_index = std::type_index(typeid(T));
            auto it = components_.find(type_index);
            if (it != components_.end())
            {
                it->second->clean();
                components_.erase(it);
            }
        }

        void update(float delta_time, fds::Context &context);
        void render(fds::Context &context);
        void clean();
        void handleInput(fds::Context &context);
    private:
        std::string name_;
        std::string tag_;
        std::unordered_map<std::type_index, std::unique_ptr<fds::Component>> components_;
        bool need_remove_ = false;

    };

}

#endif // FDS_GAME_OBJECT_H