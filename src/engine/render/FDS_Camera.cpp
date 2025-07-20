#include "FDS_Camera.h"

fds::Camera::Camera(glm::vec2 viewport_size, glm::vec2 position, std::optional<fds::Rect> limit_bounds)
    : m_viewportSize(std::move(viewport_size)),
      m_position(std::move(position)),
      m_limitBounds(std::move(limit_bounds))
{}

void fds::Camera::update(float deltaTime)
{
    // TODO
    clampPosition();
}

void fds::Camera::move(const glm::vec2 &offset)
{
    m_position += offset;
    clampPosition();
}

glm::vec2 fds::Camera::worldToScreen(const glm::vec2 &world_pos) const
{
    return world_pos - m_position;
}

glm::vec2 fds::Camera::worldToScreenWithParallax(const glm::vec2 &world_pos, const glm::vec2 &scroll_factor) const
{
    return world_pos - m_position * scroll_factor;
}

glm::vec2 fds::Camera::screenToWorld(const glm::vec2 &screen_pos) const
{
    return screen_pos + m_position;
}

void fds::Camera::setPosition(glm::vec2 position)
{
    m_position = std::move(position);
    clampPosition();
}

void fds::Camera::setLimitBounds(std::optional<fds::Rect> limit_bounds)
{
    m_limitBounds = std::move(limit_bounds);
    clampPosition();
}

const glm::vec2 &fds::Camera::getPosition() const
{
    return m_position;
}

std::optional<fds::Rect> fds::Camera::getLimitBounds() const
{
    return m_limitBounds;
}

glm::vec2 fds::Camera::getViewportSize() const
{
    return m_viewportSize;
}

void fds::Camera::clampPosition()
{
    if (m_limitBounds.has_value() && m_limitBounds->size.x > 0 && m_limitBounds->size.y > 0)
    {
        glm::vec2 min_cam_pos = m_limitBounds->pos;
        glm::vec2 max_cam_pos = m_limitBounds->pos + m_limitBounds->size - m_viewportSize;

        max_cam_pos.x = std::max(min_cam_pos.x, max_cam_pos.x);
        max_cam_pos.y = std::max(min_cam_pos.y, max_cam_pos.y);
        m_position = glm::clamp(m_position, min_cam_pos, max_cam_pos);
    }
}
