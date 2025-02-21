#pragma once

#include "Util.h"
#include "Entity.h"

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Game
{
public:

	void Init();
	void Setting();
	void MenuScreen();
	void Update();
	void Finalize();

	// use this function while rendering
	void printTTF(const char* text, int size, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int posX, int posY);

	bool checkCollision(SDL_Rect rect1, SDL_Rect rect2);
	Side checkWallCollision(SDL_Rect rect);

	bool restart = false;

private:

	SDL_Rect windowSize{ 0, 0, 720, 960 };

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;
	 
	unsigned FPS = 120;
	unsigned frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	bool isRunning = true;

	ScrollingEntity gameBackground{};

	Entity player{};
	Entity enemy{};

	BulletEntity playerBullet{};
	BulletEntity enemyBullet{};

	Mix_Music* inGameBGM;
	Mix_Chunk* bulletShootSoundEffect;
	Mix_Chunk* bulletHitSoundEffect;
	Mix_Chunk* gameoverEffect;
	
	int score;

	int random;
};

