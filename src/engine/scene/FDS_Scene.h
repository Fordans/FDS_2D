#ifndef FDS_SCENE_H
#define FDS_SCENE_H

#include <vector>
#include <memory>
#include <string>
#include <string_view>

namespace fds
{
    class Context;
    class UIManager;
    class GameObject;
    class SceneManager;

    class Scene
    {
    protected:
        std::string scene_name_;
        fds::Context &context_;
        fds::SceneManager &scene_manager_;
        std::unique_ptr<fds::UIManager> ui_manager_;

        bool is_initialized_ = false;
        std::vector<std::unique_ptr<fds::GameObject>> game_objects_;
        std::vector<std::unique_ptr<fds::GameObject>> pending_additions_;

    public:
        Scene(std::string_view name, fds::Context &context, fds::SceneManager &scene_manager);

        virtual ~Scene();

        Scene(const Scene &) = delete;
        Scene &operator=(const Scene &) = delete;
        Scene(Scene &&) = delete;
        Scene &operator=(Scene &&) = delete;

        virtual void init();
        virtual void update(float delta_time); 
        virtual void render();
        virtual void handleInput();
        virtual void clean();

        virtual void addGameObject(std::unique_ptr<fds::GameObject> &&game_object);

        virtual void safeAddGameObject(std::unique_ptr<fds::GameObject> &&game_object);

        virtual void removeGameObject(fds::GameObject *game_object_ptr);

        virtual void safeRemoveGameObject(fds::GameObject *game_object_ptr);

        const std::vector<std::unique_ptr<fds::GameObject>> &getGameObjects() const { return game_objects_; }

        fds::GameObject *findGameObjectByName(std::string_view name) const;

        // getters and setters
        void setName(std::string_view name) { scene_name_ = name; }              
        std::string_view getName() const { return scene_name_; }                 
        void setInitialized(bool initialized) { is_initialized_ = initialized; } 
        bool isInitialized() const { return is_initialized_; }                   

        fds::Context &getContext() const { return context_; }                                    
        fds::SceneManager &getSceneManager() const { return scene_manager_; }                    
        std::vector<std::unique_ptr<fds::GameObject>> &getGameObjects() { return game_objects_; }

    protected:
        void processPendingAdditions();
    };
}

#endif // FDS_SCENE_H