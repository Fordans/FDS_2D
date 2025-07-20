#include "FDS_Game.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "engine/render/FDS_Renderer.h"

#include "SDL3/SDL.h"
#include "spdlog/spdlog.h"

fds::Game::Game()
{
    m_time = std::make_unique<fds::Time>();
    m_windowSize = glm::vec2(1600.0f, 1200.0f);
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

    m_time->setTargetFPS(60);
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
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        spdlog::error("Failed to initialize SDL: {}", SDL_GetError());
        return false;
    }

    m_window = SDL_CreateWindow("FDS_Game", int(m_windowSize.x), int(m_windowSize.y), 0);
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

    try
    {
        m_resourceManager = std::make_unique<fds::ResourceManager>(m_renderer);
    } 
    catch (const std::exception& e)
    {
        spdlog::error("Failed to init ResourceManager: {}", e.what());
        return false;
    }

    try
    {
        m_fdsRenderer = std::make_unique<fds::Renderer>(m_renderer, m_resourceManager.get());
    } 
    catch (const std::exception& e)
    {
        spdlog::error("Failed to init fdsRenderer: {}", e.what());
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
