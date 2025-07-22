#include "FDS_Transform_Component.h"
#include "FDS_Sprite_Component.h"
#include "engine/object/FDS_GameObject.h"

namespace fds
{
    void TransformComponent::setScale(glm::vec2 scale)
    {
        scale_ = std::move(scale);
        if (owner_)
        {
            auto sprite_comp = owner_->getComponent<SpriteComponent>();
            if (sprite_comp)
            {
                sprite_comp->updateOffset();
            }
            // auto collider_comp = owner_->getComponent<ColliderComponent>();
            // if (collider_comp)
            // {
            //     collider_comp->updateOffset();
            // }
        }
    }
}