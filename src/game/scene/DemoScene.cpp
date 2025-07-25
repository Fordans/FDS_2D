#include "DemoScene.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/render/FDS_Renderer.h"

#include "glm/glm.hpp"

DemoScene::DemoScene(fds::Context &context, fds::SceneManager &sceneManager)
    : fds::Scene("DemoScene", context, sceneManager)
{

}

void DemoScene::init()
{
    // test stuff
    context_.getResourceManager().loadTexture("res/demo/Fordans.png");
    testSprite_ = fds::Sprite("res/demo/Fordans.png");
}

void DemoScene::clean()
{
    Scene::clean();
}

void DemoScene::update(float deltaTime)
{
    Scene::update(deltaTime);
}

void DemoScene::render()
{
    // test stuff
    context_.getRenderer().drawUISprite(testSprite_, glm::vec2(0, 0));
    Scene::render();
}
