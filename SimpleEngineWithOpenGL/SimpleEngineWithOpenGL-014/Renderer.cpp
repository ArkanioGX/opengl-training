#include "Renderer.h"
#include "Log.h"
#include "Texture.h"
#include "Maths.h"

Renderer::Renderer() : SDLRenderer(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::initialize(Window& window)
{
	SDLRenderer = SDL_CreateRenderer(window.getSDLWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!SDLRenderer)
	{
		Log::error(SDL_LOG_CATEGORY_VIDEO, "Failed to create renderer");
		return false;
	}
	return true;
}

void Renderer::beginDraw()
{
	SDL_SetRenderDrawColor(SDLRenderer, 120, 120, 255, 255);
	SDL_RenderClear(SDLRenderer);
}

void Renderer::endDraw()
{
	SDL_RenderPresent(SDLRenderer);
}

void Renderer::drawRect(const Rectangle& rect) const
{
	SDL_SetRenderDrawColor(SDLRenderer, 255, 255, 255, 255);
	SDL_Rect SDLRect = rect.toSDLRect();
	SDL_RenderFillRect(SDLRenderer, &SDLRect);
}

void Renderer::drawSprite(Vector2 position, float rotation, float scale, const Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const
{
	SDL_Rect dstRect;
	// Scale the width/height by owner's scale
	dstRect.w = static_cast<int>(tex.getWidth() * scale);
	dstRect.h = static_cast<int>(tex.getHeight() * scale);
	// Center the rectangle around the position of the owner
	dstRect.x = static_cast<int>(position.x - origin.x);
	dstRect.y = static_cast<int>(position.y - origin.y);

	SDL_Rect* srcSDL = nullptr;
	if (srcRect != Rectangle::nullRect)
	{
		srcSDL = new SDL_Rect {
			Maths::round(srcRect.x),
			Maths::round(srcRect.y),
			Maths::round(srcRect.width),
			Maths::round(srcRect.height)
		};
	}

	SDL_RenderCopyEx(
		SDLRenderer,
		tex.toSDLTexture(),
		srcSDL,
		&dstRect,
		-Maths::toDegrees(rotation),
		nullptr,		// rotation point, center by default
		SDL_FLIP_NONE);

	delete srcSDL;
}

void Renderer::close()
{
	SDL_DestroyRenderer(SDLRenderer);
}
