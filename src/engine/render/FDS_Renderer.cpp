#include "FDS_Renderer.h"
#include "engine/resource/FDS_ResourceManager.h"
#include "FDS_Sprite.h"
#include "FDS_Camera.h"

#include "spdlog/spdlog.h"

fds::Renderer::Renderer(SDL_Renderer *sdl_renderer, fds::ResourceManager *resource_manager)
    : m_renderer(sdl_renderer), m_resourceManager(resource_manager)
{
    setDrawColor(0, 0, 0, 255);
}

void fds::Renderer::drawSprite(const Camera &camera, const Sprite &sprite, const glm::vec2 &position, const glm::vec2 &scale, double angle)
{
    auto texture = m_resourceManager->getTexture(sprite.getId());
    if (!texture)
    {
        spdlog::error("Failed to get texture, id : {}", sprite.getId());
        return;
    }

    auto src_rect = getSpriteSrcRect(sprite);
    if (!src_rect.has_value())
    {
        spdlog::error("Failed to get srcRect, id : {}", sprite.getId());
        return;
    }

    glm::vec2 position_screen = camera.worldToScreen(position);

    float scaled_w = src_rect.value().w * scale.x;
    float scaled_h = src_rect.value().h * scale.y;
    SDL_FRect dest_rect = {
        position_screen.x,
        position_screen.y,
        scaled_w,
        scaled_h};

    if (!isRectInViewport(camera, dest_rect))
    {
        return;
    }

    if (!SDL_RenderTextureRotated(m_renderer, texture, &src_rect.value(), &dest_rect, angle, NULL, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
    {
        spdlog::error("Failed to set texture filp,ID: {} : {}", sprite.getId(), SDL_GetError());
    }
}

void fds::Renderer::drawParallax(const Camera &camera, const Sprite &sprite, const glm::vec2 &position, const glm::vec2 &scroll_factor, glm::bvec2 repeat, const glm::vec2 &scale)
{
    auto texture = m_resourceManager->getTexture(sprite.getId());
    if (!texture)
    {
        spdlog::error("Failed to get texture, id : {}", sprite.getId());
        return;
    }

    auto src_rect = getSpriteSrcRect(sprite);
    if (!src_rect.has_value())
    {
        spdlog::error("Failed to get srcRect, id : {}", sprite.getId());
        return;
    }

    glm::vec2 position_screen = camera.worldToScreenWithParallax(position, scroll_factor);

    float scaled_tex_w = src_rect.value().w * scale.x;
    float scaled_tex_h = src_rect.value().h * scale.y;

    glm::vec2 start, stop;
    glm::vec2 viewport_size = camera.getViewportSize();

    if (repeat.x)
    {
        start.x = glm::mod(position_screen.x, scaled_tex_w) - scaled_tex_w;
        stop.x = viewport_size.x;
    }
    else
    {
        start.x = position_screen.x;
        stop.x = glm::min(position_screen.x + scaled_tex_w, viewport_size.x);
    }
    if (repeat.y)
    {
        start.y = glm::mod(position_screen.y, scaled_tex_h) - scaled_tex_h;
        stop.y = viewport_size.y;
    }
    else
    {
        start.y = position_screen.y;
        stop.y = glm::min(position_screen.y + scaled_tex_h, viewport_size.y);
    }

    for (float y = start.y; y < stop.y; y += scaled_tex_h)
    {
        for (float x = start.x; x < stop.x; x += scaled_tex_w)
        {
            SDL_FRect dest_rect = {x, y, scaled_tex_w, scaled_tex_h};
            if (!SDL_RenderTexture(m_renderer, texture, nullptr, &dest_rect))
            {
                spdlog::error("Failed to render parallax, id : {}, {}", sprite.getId(), SDL_GetError());
                return;
            }
        }
    }
}

void fds::Renderer::drawUISprite(const Sprite &sprite, const glm::vec2 &position, const std::optional<glm::vec2> &size)
{
    auto texture = m_resourceManager->getTexture(sprite.getId());
    if (!texture)
    {
        spdlog::error("Failed to get texture, id : {}", sprite.getId());
        return;
    }

    auto src_rect = getSpriteSrcRect(sprite);
    if (!src_rect.has_value())
    {
        spdlog::error("Failed to get srcRect, id : {}", sprite.getId());
        return;
    }

    SDL_FRect dest_rect = {position.x, position.y, 0, 0};
    if (size.has_value())
    {
        dest_rect.w = size.value().x;
        dest_rect.h = size.value().y;
    }
    else
    {
        dest_rect.w = src_rect.value().w;
        dest_rect.h = src_rect.value().h;
    }

    if (!SDL_RenderTextureRotated(m_renderer, texture, &src_rect.value(), &dest_rect, 0.0, nullptr, sprite.isFlipped() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE))
    {
        spdlog::error("Failed to render UI sprite, id : {}, {}", sprite.getId(), SDL_GetError());
    }
}

void fds::Renderer::drawUIFilledRect(const fds::Rect &rect, const fds::FColor &color)
{
    setDrawColorFloat(color.r, color.g, color.b, color.a);
    SDL_FRect sdl_rect = {rect.pos.x, rect.pos.y, rect.size.x, rect.size.y};
    if (!SDL_RenderFillRect(m_renderer, &sdl_rect))
    {
        spdlog::error("Failed to render Filled Rect : {}", SDL_GetError());
    }
    setDrawColor(0, 0, 0, 1);
}

void fds::Renderer::present()
{
    SDL_RenderPresent(m_renderer);
}

void fds::Renderer::clearScreen()
{
    SDL_RenderClear(m_renderer);
}

void fds::Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void fds::Renderer::setDrawColorFloat(float r, float g, float b, float a)
{
    SDL_SetRenderDrawColorFloat(m_renderer, r, g, b, a);
}

std::optional<SDL_FRect> fds::Renderer::getSpriteSrcRect(const Sprite &sprite)
{
    SDL_Texture *texture = m_resourceManager->getTexture(sprite.getId());
    if (!texture)
    {
        spdlog::error("Failed to get texture, id : {}", sprite.getId());
        return std::nullopt;
    }

    auto src_rect = sprite.getSourceRect();
    if (src_rect.has_value())
    {
        if (src_rect.value().w <= 0 || src_rect.value().h <= 0)
        {
            spdlog::error("srcRect invalid, id : {}", sprite.getId());
            return std::nullopt;
        }
        return src_rect;
    }
    else
    {
        SDL_FRect result = {0, 0, 0, 0};
        if (!SDL_GetTextureSize(texture, &result.w, &result.h))
        {
            spdlog::error("Failed to get texture size, id : {}", sprite.getId());
            return std::nullopt;
        }
        return result;
    }
}

bool fds::Renderer::isRectInViewport(const Camera &camera, const SDL_FRect &rect)
{
    glm::vec2 viewport_size = camera.getViewportSize();
    return rect.x + rect.w >= 0 && rect.x <= viewport_size.x && rect.y + rect.h >= 0 && rect.y <= viewport_size.y; // AABB
}