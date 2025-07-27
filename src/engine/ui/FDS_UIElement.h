#ifndef FDS_UI_ELEMENT_H
#define FDS_UI_ELEMENT_H

#include "engine/utility/FDS_math.h"

#include "SDL3/SDL_rect.h"

#include <memory>
#include <vector>

namespace fds
{
    class Context;

    class UIElement
    {
    protected:
        glm::vec2 position_;
        glm::vec2 size_;
        bool visible_ = true;
        bool need_remove_ = false;

        UIElement *parent_ = nullptr;
        std::vector<std::unique_ptr<UIElement>> children_;

    public:
        explicit UIElement(glm::vec2 position = {0.0f, 0.0f}, glm::vec2 size = {0.0f, 0.0f});
        virtual ~UIElement() = default;

        virtual bool handleInput(fds::Context &context);
        virtual void update(float delta_time, fds::Context &context);
        virtual void render(fds::Context &context);

        void addChild(std::unique_ptr<UIElement> child);
        std::unique_ptr<UIElement> removeChild(UIElement *child_ptr);
        void removeAllChildren();

        const glm::vec2 &getSize() const { return size_; }
        const glm::vec2 &getPosition() const { return position_; }
        bool isVisible() const { return visible_; }
        bool isNeedRemove() const { return need_remove_; }
        UIElement *getParent() const { return parent_; }
        const std::vector<std::unique_ptr<UIElement>> &getChildren() const { return children_; }

        void setSize(glm::vec2 size) { size_ = std::move(size); }
        void setVisible(bool visible) { visible_ = visible; }
        void setParent(UIElement *parent) { parent_ = parent; }
        void setPosition(glm::vec2 position) { position_ = std::move(position); }
        void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }

        fds::Rect getBounds() const;
        glm::vec2 getScreenPosition() const;
        bool isPointInside(const glm::vec2 &point) const;

        UIElement(const UIElement &) = delete;
        UIElement &operator=(const UIElement &) = delete;
        UIElement(UIElement &&) = delete;
        UIElement &operator=(UIElement &&) = delete;
    };

}

#endif // FDS_UI_ELEMENT_H