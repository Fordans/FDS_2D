#include "FDS_UIManager.h"
#include "FDS_UIPanel.h"
#include "FDS_UIElement.h"

#include "spdlog/spdlog.h"

namespace fds
{
    UIManager::~UIManager() = default;

    UIManager::UIManager()
    {
        root_element_ = std::make_unique<UIPanel>(glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f});
    }

    bool UIManager::init(const glm::vec2 &window_size)
    {
        root_element_->setSize(window_size);
        return true;
    }

    void UIManager::addElement(std::unique_ptr<UIElement> element)
    {
        if (root_element_)
        {
            root_element_->addChild(std::move(element));
        }
        else
        {
            spdlog::error("Failed to add element to UIManager: root element is null");
        }
    }

    void UIManager::clearElements()
    {
        if (root_element_)
        {
            root_element_->removeAllChildren();
            spdlog::trace("All elements removed from UIManager");
        }
    }

    bool UIManager::handleInput(fds::Context &context)
    {
        if (root_element_ && root_element_->isVisible())
        {
            if (root_element_->handleInput(context))
                return true;
        }
        return false;
    }

    void UIManager::update(float delta_time, fds::Context &context)
    {
        if (root_element_ && root_element_->isVisible())
        {
            root_element_->update(delta_time, context);
        }
    }

    void UIManager::render(fds::Context &context)
    {
        if (root_element_ && root_element_->isVisible())
        {
            root_element_->render(context);
        }
    }

    UIPanel *UIManager::getRootElement() const
    {
        return root_element_.get();
    }

}