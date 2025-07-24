#ifndef FDS_TEXT_RENDERER_H
#define FDS_TEXT_RENDERER_H

#include "engine/utility/FDS_math.h"

#include "SDL3/SDL_render.h"
#include "glm/glm.hpp"

#include <string>
#include <string_view>

struct TTF_TextEngine;

namespace fds
{
    class ResourceManager;
    class Camera;

    class TextRenderer final
    {
    private:
        SDL_Renderer *sdl_renderer_ = nullptr;
        fds::ResourceManager *resource_manager_ = nullptr;

        TTF_TextEngine *text_engine_ = nullptr;
    public:
        TextRenderer(SDL_Renderer *sdl_renderer, fds::ResourceManager *resource_manager);

        ~TextRenderer();

        void close();

        void drawUIText(std::string_view text, std::string_view font_id, int font_size,
                        const glm::vec2 &position, const fds::FColor &color = {1.0f, 1.0f, 1.0f, 1.0f});

        void drawText(const Camera &camera, std::string_view text, std::string_view font_id, int font_size,
                      const glm::vec2 &position, const fds::FColor &color = {1.0f, 1.0f, 1.0f, 1.0f});

        glm::vec2 getTextSize(std::string_view text, std::string_view font_id, int font_size);

        TextRenderer(const TextRenderer &) = delete;
        TextRenderer &operator=(const TextRenderer &) = delete;
        TextRenderer(TextRenderer &&) = delete;
        TextRenderer &operator=(TextRenderer &&) = delete;

    };
}

#endif // FDS_TEXT_RENDERER_H