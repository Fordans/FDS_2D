#ifndef FDS_TRANSFORM_COMPONENT_H
#define FDS_TRANSFORM_COMPONENT_H

#include "FDS_Component.h"

#include "glm/glm.hpp"

#include <utility>

namespace fds
{
    class TransformComponent final : public Component
    {
        friend class fds::GameObject;

    public:
        glm::vec2 position_ = {0.0f, 0.0f};
        glm::vec2 scale_ = {1.0f, 1.0f};
        float rotation_ = 0.0f;

        TransformComponent(glm::vec2 position = {0.0f, 0.0f}, glm::vec2 scale = {1.0f, 1.0f}, float rotation = 0.0f)
            : position_(std::move(position)), scale_(std::move(scale)), rotation_(rotation) {}

        TransformComponent(const TransformComponent &) = delete;
        TransformComponent &operator=(const TransformComponent &) = delete;
        TransformComponent(TransformComponent &&) = delete;
        TransformComponent &operator=(TransformComponent &&) = delete;

        // Getters and setters
        const glm::vec2 &getPosition() const { return position_; }
        float getRotation() const { return rotation_; }
        const glm::vec2 &getScale() const { return scale_; }
        void setPosition(glm::vec2 position) { position_ = std::move(position); }
        void setRotation(float rotation) { rotation_ = rotation; }
        void setScale(glm::vec2 scale);
        void translate(const glm::vec2 &offset) { position_ += offset; }

    private:
        void update(float, fds::Context &) override {}
    };

}

#endif // FDS_TRANSFORM_COMPONENT_H