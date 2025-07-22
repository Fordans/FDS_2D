#ifndef FDS_SPRITE_COMPONENT_H
#define FDS_SPRITE_COMPONENT_H

#include "FDS_Component.h"
#include "engine/render/FDS_Sprite.h"
#include "engine/utility/FDS_alignment.h"

#include "glm/glm.hpp"
#include "SDL3/SDL_rect.h"

#include <string>
#include <string_view>
#include <optional>

namespace fds
{
    class ResourceManager;
    class Context;
    class TransformComponent;

    class SpriteComponent final : public fds::Component
    {
        friend class fds::GameObject;

    private:
        fds::ResourceManager *resource_manager_ = nullptr;
        TransformComponent *transform_ = nullptr;

        fds::Sprite sprite_;
        fds::Alignment alignment_ = fds::Alignment::NONE;
        glm::vec2 sprite_size_ = {0.0f, 0.0f};
        glm::vec2 offset_ = {0.0f, 0.0f};
        bool is_hidden_ = false;

    public:
        SpriteComponent(
            std::string_view texture_id,
            fds::ResourceManager &resource_manager,
            fds::Alignment alignment = fds::Alignment::NONE,
            std::optional<SDL_FRect> source_rect_opt = std::nullopt,
            bool is_flipped = false);

        SpriteComponent(
            fds::Sprite &&sprite,
            fds::ResourceManager &resource_manager,
            fds::Alignment alignment = fds::Alignment::NONE);

        ~SpriteComponent() override = default;

        SpriteComponent(const SpriteComponent &) = delete;
        SpriteComponent &operator=(const SpriteComponent &) = delete;
        SpriteComponent(SpriteComponent &&) = delete;
        SpriteComponent &operator=(SpriteComponent &&) = delete;

        void updateOffset();

        // Getters
        const fds::Sprite &getSprite() const { return sprite_; }
        std::string_view getTextureId() const { return sprite_.getId(); }
        bool isFlipped() const { return sprite_.isFlipped(); }
        bool isHidden() const { return is_hidden_; }
        const glm::vec2 &getSpriteSize() const { return sprite_size_; }
        const glm::vec2 &getOffset() const { return offset_; }
        fds::Alignment getAlignment() const { return alignment_; }

        // Setters
        void setSpriteById(std::string_view texture_id, std::optional<SDL_FRect> source_rect_opt = std::nullopt);
        void setFlipped(bool flipped) { sprite_.setFlipped(flipped); }
        void setHidden(bool hidden) { is_hidden_ = hidden; }
        void setSourceRect(std::optional<SDL_FRect> source_rect_opt);
        void setAlignment(fds::Alignment anchor);

    private:
        void updateSpriteSize();

        // Component virtual functions merge
        void init() override;
        void update(float, fds::Context &) override {}
        void render(fds::Context &context) override;
    };
}

#endif // FDS_SPRITE_COMPONENT_H