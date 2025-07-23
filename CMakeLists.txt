cmake_minimum_required(VERSION 3.10.0)
project(FDS_2D VERSION 0.1.0 LANGUAGES C CXX)

# compile with utf-8 for MSVC spdlog
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src)
include_directories(include)
link_directories(lib)

add_executable(
    FDS_2D
src/engine/component/FDS_Component.h
src/engine/component/FDS_Transform_Component.cpp
src/engine/component/FDS_Sprite_Component.cpp
src/engine/core/FDS_main.cpp
src/engine/core/FDS_Game.cpp
src/engine/core/FDS_Time.cpp
src/engine/core/FDS_Config.cpp
src/engine/core/FDS_Context.cpp
src/engine/input/FDS_InputManager.cpp
src/engine/object/FDS_GameObject.cpp
src/engine/render/FDS_Camera.cpp
src/engine/render/FDS_Sprite.h
src/engine/render/FDS_Renderer.cpp
src/engine/resource/FDS_TextureManager.cpp
src/engine/resource/FDS_AudioManager.cpp
src/engine/resource/FDS_FontManager.cpp
src/engine/resource/FDS_ResourceManager.cpp
src/engine/scene/FDS_Scene.cpp
src/engine/scene/FDS_SceneManager.cpp
src/engine/utility/FDS_alignment.h
src/engine/utility/FDS_math.h

src/game/scene/TestScene.cpp
)

target_link_options(
    FDS_2D
PRIVATE
$<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
$<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
)

target_link_libraries(
    FDS_2D
SDL3
SDL3_image
SDL3_mixer
SDL3_ttf
)

