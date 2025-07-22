#include "FDS_Game.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/render/FDS_Renderer.h"
#include "engine/render/FDS_Camera.h"
#include "engine/core/FDS_Config.h"
#include "engine/input/FDS_InputManager.h"
#include "engine/core/FDS_Context.h"

#include "SDL3/SDL.h"
#include "spdlog/spdlog.h"

fds::Game::Game()
{
    m_time = std::make_unique<fds::Time>();
}

fds::Game::~Game()
{
    if(m_isRunning)
    {
        spdlog::warn("Exceptionally cleaning up the game resources.");
        clean();
    }
}

void fds::Game::run()
{
    if(!init())
    {
        spdlog::error("Failed to initialize the game.");
        return;
    }

    m_time->setTargetFPS(m_config->target_fps_);
    while(m_isRunning)
    {
        m_time->update();
        float deltaTime = m_time->getDeltaTime();
        handleEvents();
        update(deltaTime);
        render();
        //spdlog::info("Current FPS: {:.9f}", 1.0 / deltaTime);
    }

    clean();
}

void fds::Game::shutdown() noexcept
{
    m_isRunning = false;
}

bool fds::Game::init()
{
    // Init fds::Config
    try
    {   
        std::string_view file_path = "config.json";
        m_config = std::make_unique<fds::Config>(file_path);
    } 
    catch (const std::exception& e)
    {
        spdlog::error("Failed to init Config: {}", e.what());
        return false;
    }
    // Init SDL3
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        spdlog::error("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow(m_config->window_title_.c_str(), m_config->window_width_, m_config->window_height_, 0);
    if(m_window == nullptr)
    {
        spdlog::error("Failed to create SDL window: {}", SDL_GetError());
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, 0);
    if(m_renderer == nullptr)
    {
        spdlog::error("Failed to create SDL renderer: {}", SDL_GetError());
        SDL_DestroyWindow(m_window);
        return false;
    }
    // Init fds::ResourceManager
    try
    {
        m_resourceManager = std::make_unique<fds::ResourceManager>(m_renderer);
    } 
    catch (const std::exception& e)
    {
        spdlog::error("Failed to init ResourceManager: {}", e.what());
        return false;
    }
    // Init fds::Renderer
    try
    {
        m_fdsRenderer = std::make_unique<fds::Renderer>(m_renderer, m_resourceManager.get());
    } 
    catch (const std::exception& e)
    {
        spdlog::error("Failed to init fdsRenderer: {}", e.what());
        return false;
    }
    // Init fds::InputManager
    try
    {
        m_inputManager = std::make_unique<fds::InputManager>(m_renderer, m_config.get());
    }
    catch(const std::exception& e)
    {
        spdlog::error("Failed to init InputManager: {}", e.what());
        return false;
    }
    //Init fds:Camera
    try
    {
        m_camera = std::make_unique<fds::Camera>(glm::vec2(m_config->window_width_ / 2, m_config->window_height_ / 2));
    }
    catch(const std::exception& e)
    {
        spdlog::error("Failed to init Camera: {}", e.what());
        return false;
    }
    // Init fds::Context
    try
    {
        m_context = std::make_unique<fds::Context>(*m_inputManager, *m_fdsRenderer,  *m_camera, *m_resourceManager);
    }
    catch(const std::exception& e)
    {
        spdlog::error("Failed to init Context: {}", e.what());
        return false;
    }

    m_isRunning = true;
    return true;
}

void fds::Game::clean()
{
    if(m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
    }

    if(m_window != nullptr)
    {
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();

    spdlog::info("Game resources cleaned up successfully.");
}

void fds::Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            shutdown();
        }
    }
}

void fds::Game::update(float deltaTime)
{
}

void fds::Game::render()
{
}
