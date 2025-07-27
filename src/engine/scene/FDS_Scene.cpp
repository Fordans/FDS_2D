#include "FDS_Scene.h"
#include "FDS_SceneManager.h"
#include "engine/object/FDS_GameObject.h"
#include "engine/render/FDS_Camera.h"
#include "engine/core/FDS_Context.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/core/FDS_GameState.h"
#include "engine/physics/FDS_PhysicsEngine.h"
#include "engine/ui/FDS_UIManager.h"

#include "spdlog/spdlog.h"

#include <algorithm>

namespace fds
{
    Scene::Scene(std::string_view name, fds::Context &context, fds::SceneManager &scene_manager)
        : scene_name_(name),
          context_(context),
          scene_manager_(scene_manager),
          ui_manager_(std::make_unique<fds::UIManager>()),
          is_initialized_(false)
    {
        spdlog::trace("Scene {} constructed", scene_name_);
    }

    Scene::~Scene() = default;

    void Scene::init()
    {
        is_initialized_ = true;
        spdlog::trace("Scene {} constructed", scene_name_);
    }

    void Scene::update(float delta_time)
    {
        if (!is_initialized_)
            return;

        if (context_.getGameState().isPlaying())
        {
            context_.getPhysicsEngine().update(delta_time);
            context_.getCamera().update(delta_time);
        }

        bool need_remove = false;

        for (auto &obj : game_objects_)
        {
            if (obj && !obj->isNeedRemove())
            {
                obj->update(delta_time, context_);
            }
            else
            {
                need_remove = true;
                if (obj)
                    obj->clean();
                else
                    spdlog::warn("Attempted to clean a null object");
            }
        }

        if (need_remove)
        {
            std::erase_if(game_objects_, [](const std::unique_ptr<fds::GameObject> &obj)
                          { return !obj || obj->isNeedRemove(); });
        }

        ui_manager_->update(delta_time, context_);

        processPendingAdditions();
    }

    void Scene::render()
    {
        if (!is_initialized_)
            return;

        for (const auto &obj : game_objects_)
        {
            if (obj)
                obj->render(context_);
        }

        ui_manager_->render(context_);
    }

    void Scene::handleInput()
    {
        if (!is_initialized_)
            return;

        if (ui_manager_->handleInput(context_))
            return;

        for (auto &obj : game_objects_)
        {
            if (obj && !obj->isNeedRemove())
            {
                obj->handleInput(context_);
            }
        }
    }

    void Scene::clean()
    {
        if (!is_initialized_)
            return;

        for (const auto &obj : game_objects_)
        {
            if (obj)
                obj->clean();
        }
        game_objects_.clear();

        is_initialized_ = false;
        spdlog::trace("Scene {} destructed", scene_name_);
    }

    void Scene::addGameObject(std::unique_ptr<fds::GameObject> &&game_object)
    {
        if (game_object)
            game_objects_.push_back(std::move(game_object));
        else
            spdlog::warn("Attempted to add null GameObject to scene '{}'", scene_name_);
    }

    void Scene::safeAddGameObject(std::unique_ptr<fds::GameObject> &&game_object)
    {
        if (game_object)
            pending_additions_.push_back(std::move(game_object));
        else
            spdlog::warn("Attempted to add null GameObject to scene '{}'", scene_name_);
    }

    void Scene::removeGameObject(fds::GameObject *game_object_ptr)
    {
        if (!game_object_ptr)
        {
            spdlog::warn("Attempted to remove null GameObject from scene '{}'", scene_name_);
            return;
        }

        auto it = std::remove_if(game_objects_.begin(), game_objects_.end(),
                                 [game_object_ptr](const std::unique_ptr<fds::GameObject> &p)
                                 {
                                     return p.get() == game_object_ptr;
                                 });

        if (it != game_objects_.end())
        {
            (*it)->clean();
            game_objects_.erase(it, game_objects_.end());
            spdlog::trace("Removed GameObject from scene '{}'", scene_name_);
        }
        else
        {
            spdlog::warn("GameObject not found in Scene '{}'", scene_name_);
        }
    }

    void Scene::safeRemoveGameObject(fds::GameObject *game_object_ptr)
    {
        game_object_ptr->setNeedRemove(true);
    }

    fds::GameObject *Scene::findGameObjectByName(std::string_view name) const
    {
        for (const auto &obj : game_objects_)
        {
            if (obj && obj->getName() == name)
            {
                return obj.get();
            }
        }
        return nullptr;
    }

    void Scene::processPendingAdditions()
    {
        for (auto &game_object : pending_additions_)
        {
            addGameObject(std::move(game_object));
        }
        pending_additions_.clear();
    }
}