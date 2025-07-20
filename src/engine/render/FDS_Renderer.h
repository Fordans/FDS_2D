#ifndef FDS_RENDERER_H
#define FDS_RENDERER_H

#include "FDS_Sprite.h"
#include "engine/utility/FDS_math.h"

#include <string>
#include <optional>

struct SDL_Renderer;
struct SDL_FRect;
struct SDL_FColor;

namespace fds
{
    class Camera;
    class ResourceManager;

    class Renderer final
    {
    public:
        Renderer(SDL_Renderer* sdl_renderer, fds::ResourceManager* resource_manager);
        ~Renderer() = default;

        void drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale = {1.0f, 1.0f}, double angle = 0.0f);
        void drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scroll_factor, glm::bvec2 repeat = {true, true}, const glm::vec2& scale = {1.0f, 1.0f});
        void drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size = std::nullopt);
        void drawUIFilledRect(const fds::Rect& rect, const fds::FColor& color);
        void present();
        void clearScreen();
        void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
        void setDrawColorFloat(float r, float g, float b, float a = 1.0f);
        SDL_Renderer* getSDLRenderer() const { return m_renderer; }

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;
    private:
        std::optional<SDL_FRect> getSpriteSrcRect(const Sprite& sprite);
        bool isRectInViewport(const Camera& camera, const SDL_FRect& rect);
    private:
        SDL_Renderer *m_renderer = nullptr;
        fds::ResourceManager *m_resourceManager = nullptr;
    };
}

#endif // FDS_RENDERER_H