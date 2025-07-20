#ifndef FDS_CAMERA_H
#define FDS_CAMERA_H

#include "engine/utility/FDS_math.h"

#include <optional>

namespace fds
{
    class Camera final
    {
    public:
        Camera(glm::vec2 viewport_size, 
           glm::vec2 position = glm::vec2(0.0f, 0.0f), 
           std::optional<fds::Rect> limit_bounds = std::nullopt);
        ~Camera() = default;

        void update(float deltaTime);
        void move(const glm::vec2& offset);

        glm::vec2 worldToScreen(const glm::vec2& world_pos) const;
        glm::vec2 worldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& scroll_factor) const;
        glm::vec2 screenToWorld(const glm::vec2& screen_pos) const;

        void setPosition(glm::vec2 position);
        void setLimitBounds(std::optional<fds::Rect> limit_bounds);

        const glm::vec2& getPosition() const;
        std::optional<fds::Rect> getLimitBounds() const;
        glm::vec2 getViewportSize() const;

        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera(Camera&&) = delete;
        Camera& operator=(Camera&&) = delete;
    private:
        void clampPosition();
    private:
        glm::vec2 m_viewportSize;
        glm::vec2 m_position;
        std::optional<fds::Rect> m_limitBounds;
        float m_smoothSpeed = 5.0f;
    };
}

#endif // FDS_CAMERA_H