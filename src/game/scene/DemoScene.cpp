#include "DemoScene.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/render/FDS_Camera.h"
#include "engine/audio/FDS_AudioPlayer.h"

#include "glm/glm.hpp"
#include "spdlog/spdlog.h"

#include <functional>

DemoScene::DemoScene(fds::Context &context, fds::SceneManager &sceneManager)
    : fds::Scene("DemoScene", context, sceneManager)
{
}

void DemoScene::init()
{
    // test stuff
    context_.getResourceManager().loadMusic("res/demo/TFM-003O_02.mp3");
    context_.getResourceManager().loadTexture("res/demo/bg.png");
    testSprite_ = fds::Sprite("res/demo/bg.png");

    timer_.wait(1000, [this]()
                { context_.getAudioPlayer().playMusic("res/demo/TFM-003O_02.mp3"); });

    for (int i = 1; i < 5; ++i)
    {
        timer_.wait(1000 * i, [=]()
                    { context_.getAudioPlayer().setMusicVolume(1 - (i * 0.2f)); });
        timer_.wait(5000 + 1000 * i, [=]()
                    { context_.getAudioPlayer().setMusicVolume(i * 0.2f); });
    }
}

void DemoScene::clean()
{
    // test stuff
    context_.getResourceManager().unloadMusic("res/demo/TFM-003O_02.mp3");
    context_.getResourceManager().unloadTexture("res/demo/bg.png");
    Scene::clean();
}

void DemoScene::update(float deltaTime)
{
    // test stuff
    static double i = -15;
    context_.getCamera().move(glm::vec2(i, 0));
    Scene::update(deltaTime);
    i *= 0.95;
}

void DemoScene::render()
{
    // test stuff
    context_.getRenderer().drawUISprite(testSprite_, context_.getCamera().getPosition(), glm::vec2(2620, 1080));
    Scene::render();
}
