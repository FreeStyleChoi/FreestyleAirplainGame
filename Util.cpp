#include "Game.h"

#include "Util.h"

void Game::printTTF(const char* text, int size, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int posX, int posY)
{
	TTF_Font* font = TTF_OpenFont("./resource/NeoDunggeunmoPro-Regular.ttf", size);
	if (font == NULL) { return; }

	SDL_Color fontColor = { r, g, b, a };

	SDL_Surface* surface = TTF_RenderText_Blended(font, text, fontColor);
	if (surface == NULL) {
		TTF_CloseFont(font);
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == NULL) { return; }

	SDL_Rect rect = { posX, posY, surface->w, surface->h };

	SDL_RenderCopy(renderer, texture, NULL, &rect);

	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	return;
}

bool Game::checkCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	bool Cx = rect1.x + rect1.w >= rect2.x || rect1.x <= rect2.x + rect2.w;
	bool Cy = rect1.y + rect1.h >= rect2.y || rect1.y <= rect2.y + rect2.h;
	return Cx || Cy;
}

bool Game::checkWallCollision(SDL_Rect rect)
{
	return !checkCollision(rect, windowSize);
}
