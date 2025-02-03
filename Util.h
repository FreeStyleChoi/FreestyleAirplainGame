#pragma once
#pragma warning( disable : 4244 )

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

// use this function while rendering
void printTTF(const char* text, int size, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int posX, int posY);

// check collision
bool checkCollision(SDL_Rect rect1, SDL_Rect rect2);

// wall collision
bool checkWallCollision(SDL_Rect rect);