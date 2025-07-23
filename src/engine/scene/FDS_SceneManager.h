#ifndef FDS_SCENE_MANAGER_H
#define FDS_SCENE_MANAGER_H

#include <memory>
#include <string>
#include <vector>

namespace fds
{
    class Scene;
    class Context;

    class SceneManager final
    {
    private:
        fds::Context &context_;
        std::vector<std::unique_ptr<Scene>> scene_stack_;

        enum class PendingAction
        {
            None,
            Push,
            Pop,
            Replace
        };
        PendingAction pending_action_ = PendingAction::None;
        std::unique_ptr<Scene> pending_scene_;

    public:
        explicit SceneManager(fds::Context &context);
        ~SceneManager();

        SceneManager(const SceneManager &) = delete;
        SceneManager &operator=(const SceneManager &) = delete;
        SceneManager(SceneManager &&) = delete;
        SceneManager &operator=(SceneManager &&) = delete;

        void requestPushScene(std::unique_ptr<Scene> &&scene);
        void requestPopScene();
        void requestReplaceScene(std::unique_ptr<Scene> &&scene);

        // getters
        Scene *getCurrentScene() const;
        fds::Context &getContext() const { return context_; }

        void update(float delta_time);
        void render();
        void handleInput();
        void close();
    private:
        void processPendingActions();
        void pushScene(std::unique_ptr<Scene> &&scene);   
        void popScene();                                  
        void replaceScene(std::unique_ptr<Scene> &&scene);
    };
}

#endif // FDS_SCENE_MANAGER_H