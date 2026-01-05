#pragma once

#include "engine/scene/FDS_Scene.h"
#include "engine/scene/FDS_SceneManager.h"
#include "engine/core/FDS_Context.h"
#include "engine/render/FDS_Sprite.h"
#include "engine/utility/FDS_Timer.h"

#include <memory>

class DemoScene final : public fds::Scene
{
public:
    DemoScene(fds::Context &context, fds::SceneManager &sceneManager);
    ~DemoScene() override = default;

    void init() override;
    void clean() override;
    void update(float deltaTime) override;
    void render() override;
};