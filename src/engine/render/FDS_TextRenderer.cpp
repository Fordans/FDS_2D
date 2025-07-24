#include "FDS_TextRenderer.h"
#include "FDS_Camera.h"
#include "engine/resource/FDS_ResourceManager.h"

#include "SDL3_ttf/SDL_ttf.h"
#include "spdlog/spdlog.h"

#include <stdexcept>

namespace fds
{
    TextRenderer::TextRenderer(SDL_Renderer *sdl_renderer, fds::ResourceManager *resource_manager)
        : sdl_renderer_(sdl_renderer),
          resource_manager_(resource_manager)
    {
        if (!sdl_renderer_ || !resource_manager_)
        {
            throw std::runtime_error("Failed to init TextRenderer, SDL_Renderer or ResourceManager is nullptr");
        }

        if (!TTF_WasInit() && TTF_Init() == false)
        {
            throw std::runtime_error("Failed to init TextRenderer: " + std::string(SDL_GetError()));
        }

        text_engine_ = TTF_CreateRendererTextEngine(sdl_renderer_);
        if (!text_engine_)
        {
            spdlog::error("Failed to init TextRenderer: {}", SDL_GetError());
            throw std::runtime_error("Failed to init TextRenderer");
        }
    }

    TextRenderer::~TextRenderer()
    {
        if (text_engine_)
        {
            close();
        }
    }

    void TextRenderer::close()
    {
        if (text_engine_)
        {
            TTF_DestroyRendererTextEngine(text_engine_);
            text_engine_ = nullptr;
            spdlog::trace("TTF_TextEngine destructed");
        }
        TTF_Quit();
    }

    void TextRenderer::drawUIText(std::string_view text, std::string_view font_id, int font_size,
                                  const glm::vec2 &position, const fds::FColor &color)
    {
        TTF_Font *font = resource_manager_->getFont(font_id, font_size);
        if (!font)
        {
            spdlog::warn("drawUIText Failed to get font: {} size {}", font_id, font_size);
            return;
        }

        TTF_Text *temp_text_object = TTF_CreateText(text_engine_, font, text.data(), 0);
        if (!temp_text_object)
        {
            spdlog::error("drawUIText Failed to create temp TTF_Text: {}", SDL_GetError());
            return;
        }

        TTF_SetTextColorFloat(temp_text_object, 0.0f, 0.0f, 0.0f, 1.0f);
        if (!TTF_DrawRendererText(temp_text_object, position.x + 2, position.y + 2))
        {
            spdlog::error("drawUIText Failed to create temp TTF_Text: {}", SDL_GetError());
        }

        TTF_SetTextColorFloat(temp_text_object, color.r, color.g, color.b, color.a);
        if (!TTF_DrawRendererText(temp_text_object, position.x, position.y))
        {
            spdlog::error("drawUIText Failed to create temp TTF_Text: {}", SDL_GetError());
        }

        TTF_DestroyText(temp_text_object);
    }

    void TextRenderer::drawText(const Camera &camera, std::string_view text, std::string_view font_id, int font_size,
                                const glm::vec2 &position, const fds::FColor &color)
    {
        glm::vec2 position_screen = camera.worldToScreen(position);

        drawUIText(text, font_id, font_size, position_screen, color);
    }

    glm::vec2 TextRenderer::getTextSize(std::string_view text, std::string_view font_id, int font_size)
    {
        TTF_Font *font = resource_manager_->getFont(font_id, font_size);
        if (!font)
        {
            spdlog::warn("getTextSize Failed to get font {} size {}", font_id, font_size);
            return glm::vec2(0.0f, 0.0f);
        }

        TTF_Text *temp_text_object = TTF_CreateText(text_engine_, font, text.data(), 0);
        if (!temp_text_object)
        {
            spdlog::error("getTextSize Failed to create temp TTF_Text: {}", SDL_GetError());
            return glm::vec2(0.0f, 0.0f);
        }

        int width, height;
        TTF_GetTextSize(temp_text_object, &width, &height);

        TTF_DestroyText(temp_text_object);

        return glm::vec2(static_cast<float>(width), static_cast<float>(height));
    }
}