#include "FDS_Game.h"

#include "spdlog/spdlog.h"

#ifdef _DEBUG

int main()
{
    spdlog::set_level(spdlog::level::info);

    fds::Game game;
    game.run();
}

#else

#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    spdlog::set_level(spdlog::level::off);

    fds::Game game;
    game.run();
}

#endif