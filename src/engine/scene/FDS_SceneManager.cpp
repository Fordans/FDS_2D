#include "FDS_SceneManager.h"
#include "FDS_Scene.h"
#include "engine/core/FDS_Context.h"

#include "spdlog/spdlog.h"

namespace fds
{
    SceneManager::SceneManager(fds::Context &context)
        : context_(context)
    {
        spdlog::trace("SceneManager constructed");
    }

    SceneManager::~SceneManager()
    {
        spdlog::trace("SceneManager destructed");
        close();
    }

    Scene *SceneManager::getCurrentScene() const
    {
        if (scene_stack_.empty())
        {
            return nullptr;
        }
        return scene_stack_.back().get();
    }

    void SceneManager::update(float delta_time)
    {
        Scene *current_scene = getCurrentScene();
        if (current_scene)
        {
            current_scene->update(delta_time);
        }
        processPendingActions();
    }

    void SceneManager::render()
    {
        for (const auto &scene : scene_stack_)
        {
            if (scene)
            {
                scene->render();
            }
        }
    }

    void SceneManager::handleInput()
    {
        Scene *current_scene = getCurrentScene();
        if (current_scene)
        {
            current_scene->handleInput();
        }
    }

    void SceneManager::close()
    {
        while (!scene_stack_.empty())
        {
            if (scene_stack_.back())
            {
                spdlog::debug("Cleaning scene '{}' 。", scene_stack_.back()->getName());
                scene_stack_.back()->clean();
            }
            scene_stack_.pop_back();
        }
    }

    void SceneManager::requestPopScene()
    {
        pending_action_ = PendingAction::Pop;
    }

    void SceneManager::requestReplaceScene(std::unique_ptr<Scene> &&scene)
    {
        pending_action_ = PendingAction::Replace;
        pending_scene_ = std::move(scene);
    }

    void SceneManager::requestPushScene(std::unique_ptr<Scene> &&scene)
    {
        pending_action_ = PendingAction::Push;
        pending_scene_ = std::move(scene);
    }

    void SceneManager::processPendingActions()
    {
        if (pending_action_ == PendingAction::None)
        {
            return;
        }

        switch (pending_action_)
        {
        case PendingAction::Pop:
            popScene();
            break;
        case PendingAction::Replace:
            replaceScene(std::move(pending_scene_));
            break;
        case PendingAction::Push:
            pushScene(std::move(pending_scene_));
            break;
        default:
            break;
        }

        pending_action_ = PendingAction::None;
    }

    void SceneManager::pushScene(std::unique_ptr<Scene> &&scene)
    {
        if (!scene)
        {
            spdlog::warn("Attempted to push a null scene.");
            return;
        }
        spdlog::debug("Pushing scene '{}'", scene->getName());

        if (!scene->isInitialized())
        {
            scene->init();
        }

        scene_stack_.push_back(std::move(scene));
    }

    void SceneManager::popScene()
    {
        if (scene_stack_.empty())
        {
            spdlog::warn("Attempted to pop a scene from an empty stack.");
            return;
        }
        spdlog::debug("Poping scene '{}'", scene_stack_.back()->getName());

        if (scene_stack_.back())
        {
            scene_stack_.back()->clean();
        }
        scene_stack_.pop_back();
    }

    void SceneManager::replaceScene(std::unique_ptr<Scene> &&scene)
    {
        if (!scene)
        {
            spdlog::warn("Attempted to replace with a null scene.");
            return;
        }
        spdlog::debug("Replacing scene '{}' with '{}'", scene->getName(), scene_stack_.back()->getName());

        while (!scene_stack_.empty())
        {
            if (scene_stack_.back())
            {
                scene_stack_.back()->clean();
            }
            scene_stack_.pop_back();
        }

        if (!scene->isInitialized())
        {
            scene->init();
        }

        scene_stack_.push_back(std::move(scene));
    }

}