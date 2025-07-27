#ifndef FDS_UI_MANAGER_H
#define FDS_UI_MANAGER_H

#include "glm/vec2.hpp"

#include <memory>
#include <vector>

namespace fds
{
    class Context;
    class UIElement;
    class UIPanel;

    class UIManager final
    {
    private:
        std::unique_ptr<UIPanel> root_element_;

    public:
        UIManager();
        ~UIManager();

        [[nodiscard]] bool init(const glm::vec2 &window_size);
        void addElement(std::unique_ptr<UIElement> element);
        UIPanel *getRootElement() const;
        void clearElements();

        bool handleInput(fds::Context &);
        void update(float delta_time, fds::Context &);
        void render(fds::Context &);

        UIManager(const UIManager &) = delete;
        UIManager &operator=(const UIManager &) = delete;
        UIManager(UIManager &&) = delete;
        UIManager &operator=(UIManager &&) = delete;
    };

}

#endif // FDS_UI_MANAGER_H