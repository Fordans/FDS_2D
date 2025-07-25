#include "FDS_Game.h"
#include "FDS_GameState.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/render/FDS_Camera.h"
#include "engine/core/FDS_Config.h"
#include "engine/input/FDS_InputManager.h"
#include "engine/core/FDS_Context.h"
#include "engine/scene/FDS_SceneManager.h"
#include "engine/render/FDS_TextRenderer.h"
#include "engine/audio/FDS_AudioPlayer.h"

#include "SDL3/SDL.h"
#include "spdlog/spdlog.h"

fds::Game::Game()
{
    time_ = std::make_unique<fds::Time>();
}

fds::Game::~Game()
{
    if (isRunning_)
    {
        spdlog::warn("Exceptionally cleaning up the game resources.");
        clean();
    }
}

void fds::Game::run()
{
    if (!init())
    {
        spdlog::error("Failed to initialize the game.");
        return;
    }

    time_->setTargetFPS(config_->target_fps_);
    while (isRunning_)
    {
        time_->update();
        float deltaTime = time_->getDeltaTime();
        inputManager_->update();
        handleEvents();
        update(deltaTime);
        render();
    }

    clean();
}

void fds::Game::shutdown() noexcept
{
    isRunning_ = false;
}

void fds::Game::registerSceneSetup(std::function<void(fds::SceneManager &)> func)
{
    scene_setup_func_ = std::move(func);
}

bool fds::Game::init()
{
    // Init fds::Config
    try
    {
        std::string_view file_path = "config.json";
        config_ = std::make_unique<fds::Config>(file_path);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init Config: {}", e.what());
        return false;
    }
    // Init SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        spdlog::error("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(config_->window_title_.c_str(), config_->window_width_, config_->window_height_, 0);
    if (window_ == nullptr)
    {
        spdlog::error("Failed to create SDL window: {}", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, 0);
    if (renderer_ == nullptr)
    {
        spdlog::error("Failed to create SDL renderer: {}", SDL_GetError());
        SDL_DestroyWindow(window_);
        return false;
    }
    // Init fds::ResourceManager
    try
    {
        resourceManager_ = std::make_unique<fds::ResourceManager>(renderer_);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init ResourceManager: {}", e.what());
        return false;
    }
    // Init fds::Renderer
    try
    {
        fdsRenderer_ = std::make_unique<fds::Renderer>(renderer_, resourceManager_.get());
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init fdsRenderer: {}", e.what());
        return false;
    }
    // Init fds::InputManager
    try
    {
        inputManager_ = std::make_unique<fds::InputManager>(renderer_, config_.get());
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init InputManager: {}", e.what());
        return false;
    }
    // Init fds:Camera
    try
    {
        camera_ = std::make_unique<fds::Camera>(glm::vec2(config_->window_width_ / 2, config_->window_height_ / 2));
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init Camera: {}", e.what());
        return false;
    }
    // Init fds::Context
    try
    {
        context_ = std::make_unique<fds::Context>(*inputManager_, *fdsRenderer_, *camera_, *resourceManager_);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init Context: {}", e.what());
        return false;
    }
    // Init fds::SceneManager
    try
    {
        sceneManager_ = std::make_unique<fds::SceneManager>(*context_);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init SceneManager: {}", e.what());
        return false;
    }
    // Init AudioPlayer
    try
    {
        audioPlayer_ = std::make_unique<fds::AudioPlayer>(resourceManager_.get());
        audioPlayer_->setMusicVolume(config_->music_volume_);
        audioPlayer_->setChunkVolume(config_->chunk_volume_);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init AudioPlayer: {}", e.what());
        return false;
    }
    // Init GameState
    try
    {
        gameState_ = std::make_unique<fds::GameState>(window_, renderer_);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init GameState: {}", e.what());
        return false;
    }
    // Init TextRenderer
    try
    {
        textRenderer_ = std::make_unique<fds::TextRenderer>(renderer_, resourceManager_.get());
    }
    catch (const std::exception &e)
    {
        spdlog::error("Failed to init TextRenderer: {}", e.what());
        return false;
    }
    if(!scene_setup_func_)
    {
        spdlog::error("Scene setup function not set");
        return false;
    }
    scene_setup_func_(*sceneManager_);
    isRunning_ = true;
    return true;
}

void fds::Game::clean()
{
    if (renderer_ != nullptr)
    {
        SDL_DestroyRenderer(renderer_);
    }

    if (window_ != nullptr)
    {
        SDL_DestroyWindow(window_);
    }

    SDL_Quit();

    spdlog::debug("Game resources cleaned up successfully.");
}

void fds::Game::handleEvents()
{
    if (inputManager_->shouldQuit())
    {
        isRunning_ = false;
        return;
    }

    sceneManager_->handleInput();
}

void fds::Game::update(float deltaTime)
{
    sceneManager_->update(deltaTime);
}

void fds::Game::render()
{
    fdsRenderer_->clearScreen();
    sceneManager_->render();
    fdsRenderer_->present();
}