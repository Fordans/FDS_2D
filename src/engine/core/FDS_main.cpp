#include "FDS_Game.h"
#include "engine/scene/FDS_SceneManager.h"
#include "game/scene/DemoScene.h" // Replace with your own initial scene

#include "spdlog/spdlog.h"

void setupInitialScene(fds::SceneManager& sceneManager)
{
    // Replace DemoScene with your own initial scene
    auto scene = std::make_unique<DemoScene>(sceneManager.getContext(), sceneManager);
    sceneManager.requestPushScene(std::move(scene));
}

#ifdef _DEBUG

int main()
{
    spdlog::set_level(spdlog::level::info);

    fds::Game game;
    game.registerSceneSetup(setupInitialScene);
    game.run();
}

#else

#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    spdlog::set_level(spdlog::level::off);

    fds::Game game;
    game.registerSceneSetup(setupInitialScene);
    game.run();
}

#endif