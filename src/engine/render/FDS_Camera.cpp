#include "FDS_Camera.h"

fds::Camera::Camera(glm::vec2 viewport_size, glm::vec2 position, std::optional<fds::Rect> limit_bounds)
    : viewportSize_(std::move(viewport_size)),
      position_(std::move(position)),
      limitBounds_(std::move(limit_bounds))
{}

void fds::Camera::update(float deltaTime)
{
    // TODO
    clampPosition();
}

void fds::Camera::move(const glm::vec2 &offset)
{
    position_ += offset;
    clampPosition();
}

glm::vec2 fds::Camera::worldToScreen(const glm::vec2 &world_pos) const
{
    return world_pos - position_;
}

glm::vec2 fds::Camera::worldToScreenWithParallax(const glm::vec2 &world_pos, const glm::vec2 &scroll_factor) const
{
    return world_pos - position_ * scroll_factor;
}

glm::vec2 fds::Camera::screenToWorld(const glm::vec2 &screen_pos) const
{
    return screen_pos + position_;
}

void fds::Camera::setPosition(glm::vec2 position)
{
    position_ = std::move(position);
    clampPosition();
}

void fds::Camera::setLimitBounds(std::optional<fds::Rect> limit_bounds)
{
    limitBounds_ = std::move(limit_bounds);
    clampPosition();
}

void fds::Camera::setTarget(fds::TransformComponent *target)
{
    target_ = target;
}

const glm::vec2 &fds::Camera::getPosition() const
{
    return position_;
}

std::optional<fds::Rect> fds::Camera::getLimitBounds() const
{
    return limitBounds_;
}

glm::vec2 fds::Camera::getViewportSize() const
{
    return viewportSize_;
}

fds::TransformComponent *fds::Camera::getTarget() const
{
    return target_;
}

void fds::Camera::clampPosition()
{
    if (limitBounds_.has_value() && limitBounds_->size.x > 0 && limitBounds_->size.y > 0)
    {
        glm::vec2 min_cam_pos = limitBounds_->pos;
        glm::vec2 max_cam_pos = limitBounds_->pos + limitBounds_->size - viewportSize_;

        max_cam_pos.x = std::max(min_cam_pos.x, max_cam_pos.x);
        max_cam_pos.y = std::max(min_cam_pos.y, max_cam_pos.y);
        position_ = glm::clamp(position_, min_cam_pos, max_cam_pos);
    }
}
