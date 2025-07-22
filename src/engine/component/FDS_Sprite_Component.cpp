#include "FDS_Sprite_Component.h"
#include "FDS_Transform_Component.h"
#include "engine/object/FDS_GameObject.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/core/FDS_Context.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/render/FDS_Camera.h"

#include "spdlog/spdlog.h"

#include <stdexcept>

namespace fds
{
    SpriteComponent::SpriteComponent(
        std::string_view texture_id,
        fds::ResourceManager &resource_manager,
        fds::Alignment alignment,
        std::optional<SDL_FRect> source_rect_opt,
        bool is_flipped) : resource_manager_(&resource_manager), sprite_(texture_id, std::move(source_rect_opt), is_flipped),
                           alignment_(alignment)
    {
        if (!resource_manager_)
        {
            spdlog::critical("[SpriteComponent] ResourceManager is nullptr, invalid component");
        }
        spdlog::trace("SpriteComponent Created, ID: {}", sprite_.getId());
    }

    SpriteComponent::SpriteComponent(fds::Sprite &&sprite, fds::ResourceManager &resource_manager, fds::Alignment alignment)
        : resource_manager_(&resource_manager), sprite_(std::move(sprite)), alignment_(alignment)
    {
        if (!resource_manager_)
        {
            spdlog::critical("[SpriteComponent] ResourceManager is nullptr, invalid component");
        }
        spdlog::trace("SpriteComponent Created, ID: {}", sprite_.getId());
    }

    void SpriteComponent::init()
    {
        if (!owner_)
        {
            spdlog::error("[SpriteComponent] owner is nullptr, invalid component");
            return;
        }
        transform_ = owner_->getComponent<TransformComponent>();
        if (!transform_)
        {
            spdlog::warn(
                "on GameObject '{}' : TransformComponent required by SpriteComponent not found",
                owner_->getName());
            return;
        }

        updateSpriteSize();
        updateOffset();
    }

    void SpriteComponent::setAlignment(fds::Alignment anchor)
    {
        alignment_ = anchor;
        updateOffset();
    }

    void SpriteComponent::updateOffset()
    {
        if (sprite_size_.x <= 0 || sprite_size_.y <= 0)
        {
            offset_ = {0.0f, 0.0f};
            return;
        }
        auto scale = transform_->getScale();
        switch (alignment_)
        {
        case fds::Alignment::TOP_LEFT:
            offset_ = glm::vec2{0.0f, 0.0f} * scale;
            break;
        case fds::Alignment::TOP_CENTER:
            offset_ = glm::vec2{-sprite_size_.x / 2.0f, 0.0f} * scale;
            break;
        case fds::Alignment::TOP_RIGHT:
            offset_ = glm::vec2{-sprite_size_.x, 0.0f} * scale;
            break;
        case fds::Alignment::CENTER_LEFT:
            offset_ = glm::vec2{0.0f, -sprite_size_.y / 2.0f} * scale;
            break;
        case fds::Alignment::CENTER:
            offset_ = glm::vec2{-sprite_size_.x / 2.0f, -sprite_size_.y / 2.0f} * scale;
            break;
        case fds::Alignment::CENTER_RIGHT:
            offset_ = glm::vec2{-sprite_size_.x, -sprite_size_.y / 2.0f} * scale;
            break;
        case fds::Alignment::BOTTOM_LEFT:
            offset_ = glm::vec2{0.0f, -sprite_size_.y} * scale;
            break;
        case fds::Alignment::BOTTOM_CENTER:
            offset_ = glm::vec2{-sprite_size_.x / 2.0f, -sprite_size_.y} * scale;
            break;
        case fds::Alignment::BOTTOM_RIGHT:
            offset_ = glm::vec2{-sprite_size_.x, -sprite_size_.y} * scale;
            break;
        case fds::Alignment::NONE:
        default:
            break;
        }
    }

    void SpriteComponent::render(fds::Context &context)
    {
        if (is_hidden_ || !transform_ || !resource_manager_)
        {
            return;
        }

        const glm::vec2 &pos = transform_->getPosition() + offset_;
        const glm::vec2 &scale = transform_->getScale();
        float rotation_degrees = transform_->getRotation();

        context.getRenderer().drawSprite(context.getCamera(), sprite_, pos, scale, rotation_degrees);
    }

    void SpriteComponent::setSpriteById(std::string_view texture_id, std::optional<SDL_FRect> source_rect_opt)
    {
        sprite_.setId(texture_id);
        sprite_.setSourceRect(std::move(source_rect_opt));

        updateSpriteSize();
        updateOffset();
    }

    void SpriteComponent::setSourceRect(std::optional<SDL_FRect> source_rect_opt)
    {
        sprite_.setSourceRect(std::move(source_rect_opt));
        updateSpriteSize();
        updateOffset();
    }

    void SpriteComponent::updateSpriteSize()
    {
        if (!resource_manager_)
        {
            spdlog::error("[SpriteComponent] ResourceManager is nullptr");
            return;
        }
        if (sprite_.getSourceRect().has_value())
        {
            const auto &src_rect = sprite_.getSourceRect().value();
            sprite_size_ = {src_rect.w, src_rect.h};
        }
        else
        {
            sprite_size_ = resource_manager_->getTextureSize(sprite_.getId());
        }
    }
}