#pragma once
#pragma warning( disable : 4244 )
#pragma warning( disable : 26819 )

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#define MAXBULLET 40

class Vector
{
public:

	float x;
	float y;
};

typedef enum : int
{
	NONE = -1,
	TOP = 0,
	TOPR = 1,
	RIGHT = 2,
	BOTTOMR = 3,
	BOTTOM = 4,
	BOTTOML = 5,
	LEFT = 6,
	TOPL = 7
}Side;