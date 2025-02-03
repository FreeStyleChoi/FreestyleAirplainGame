#pragma once

#include "Util.h"

#include <array>

#include <SDL.h>

class Entity
{
public:
	SDL_Rect rect{};
	Vector speed{};
	SDL_Texture* texture{};
	bool onScreen = false;
};

class BulletEntity
{
public:
	SDL_Rect rect[MAXBULLET]{};
	Vector speed[MAXBULLET]{};
	SDL_Texture* texture{};
	bool onScreen[MAXBULLET]{};
};

class MotionlessEntity
{
public:
	SDL_Rect rect{};
	SDL_Texture* texture;
	bool onScreen = false;
};

class ScrollingEntity
{
public:
	SDL_Rect rect[2]{};
	Vector speed{};
	SDL_Texture* texture;
	bool onScreen = false;
};