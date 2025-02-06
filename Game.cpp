#include "Game.h"

void Game::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("2025", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.w, windowSize.h, SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer) { SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); }
	if (TTF_Init() == -1) { exit(-1); }

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_CHANNELS, 4, 2048);

	SDL_Surface* surface;

	// ingame background
	surface = IMG_Load("./asset/backGroundFinal.png");
	gameBackground.texture = SDL_CreateTextureFromSurface(renderer, surface);

	gameBackground.speed.x = 0;
	gameBackground.speed.y = 0.1;
	for (int i = 0; i < 2; i++)
	{
		gameBackground.rect[i].x = 0;
		gameBackground.rect[i].w = windowSize.w;
		gameBackground.rect[i].h = windowSize.h;
	}
	gameBackground.rect[0].y = 0;
	gameBackground.rect[1].y = 0 - windowSize.h;
	gameBackground.onScreen = true;

	SDL_FreeSurface(surface);

	// player
	surface = IMG_Load("./asset/player.png");
	player.texture = SDL_CreateTextureFromSurface(renderer, surface);

	player.rect.w = 46;
	player.rect.h = 43;
	player.rect.x = windowSize.w / 2 - (player.rect.w / 2);
	player.rect.y = windowSize.h - player.rect.h;
	player.speed.x = 0;
	player.speed.y = 0;
	player.onScreen = true;

	SDL_FreeSurface(surface);

	// enemy
	surface = IMG_Load("./asset/enemy.png");
	enemy.texture = SDL_CreateTextureFromSurface(renderer, surface);

	enemy.rect.w = 51;
	enemy.rect.h = 61;
	enemy.rect.x = windowSize.w / 2 - (enemy.rect.w / 2);
	enemy.rect.y = 0;
	enemy.speed.x = 0;
	enemy.speed.y = 0;
	enemy.onScreen = true;

	SDL_FreeSurface(surface);

	// player bullet
	surface = IMG_Load("");
	playerBullet.texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	for (int i = 0; i < MAXBULLET; i++)
	{
		playerBullet.rect[i].w = 16;
		playerBullet.rect[i].h = 16;
		playerBullet.rect[i].x = player.rect.x + ((player.rect.x / 2) - (playerBullet.rect[i].w / 2));
		playerBullet.rect[i].y = player.rect.y - playerBullet.rect[i].h;
	}

	SDL_FreeSurface(surface);

	// enemy bullet
	surface = IMG_Load("");
	enemyBullet.texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	for (int i = 0; i < MAXBULLET; i++)
	{
		enemyBullet.rect[i].w = 16;
		enemyBullet.rect[i].h = 16;
		enemyBullet.rect[i].x = player.rect.x + ((player.rect.x / 2) - (enemyBullet.rect[i].w / 2));
		enemyBullet.rect[i].y = player.rect.y - enemyBullet.rect[i].h;
	}

	SDL_FreeSurface(surface);
}

void Game::Update()
{
	frameStart = SDL_GetTicks64();
	while (isRunning)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{

		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:

			switch (event.key.keysym.sym)
			{

			case SDLK_ESCAPE:
				isRunning = false;
				break;

			case SDLK_w:
				player.speed.y = -0.3;
				break;
			case SDLK_UP:
				player.speed.y = -0.3;
				break;
			case SDLK_a:
				player.speed.x = -0.3;
				break;
			case SDLK_LEFT:
				player.speed.x = -0.3;
				break;
			case SDLK_s:
				player.speed.y = 0.3;
				break;
			case SDLK_DOWN:
				player.speed.y = 0.3;
				break;
			case SDLK_d:
				player.speed.x = 0.3;
				break;
			case SDLK_RIGHT:
				player.speed.x = 0.3;
				break;

			default:
				break;
			}

			break;
			
		case SDL_KEYUP:

			switch (event.key.keysym.sym)
			{

			case SDLK_ESCAPE:
				isRunning = false;
				break;

			case SDLK_w:
				player.speed.y = 0;
				break;
			case SDLK_UP:
				player.speed.y = 0;
				break;
			case SDLK_a:
				player.speed.x = 0;
				break;
			case SDLK_LEFT:
				player.speed.x = 0;
				break;
			case SDLK_s:
				player.speed.y = 0;
				break;
			case SDLK_DOWN:
				player.speed.y = 0;
				break;
			case SDLK_d:
				player.speed.x = 0;
				break;
			case SDLK_RIGHT:
				player.speed.x = 0;
				break;

			default:
				break;
			}

			break;

		default:
			break;
		}

		/////////////////////////////

		// ingmae background
		for (int i = 0; i < 2; i++)
		{
			gameBackground.rect[i].y += (int)(gameBackground.speed.y * frameDelay);
			if (gameBackground.rect[i].y >= windowSize.h) gameBackground.rect[i].y = 0 - windowSize.h;
		}

		
		// player
		player.rect.x += (int)(player.speed.x * frameDelay / 2);
		player.rect.y += (int)(player.speed.y * frameDelay / 2);


		// enemy
		float distancePlayerEnemy = (float)sqrt(pow(player.rect.x - enemy.rect.x, 2) + pow(player.rect.y - enemy.rect.y, 2));
		if (enemy.rect.y < player.rect.y)
		{
			enemy.speed.y = ((float)(player.rect.y - enemy.rect.y) * enemyDefaultSpeed) / distancePlayerEnemy;
			enemy.speed.x = ((float)(player.rect.x - enemy.rect.x) * enemyDefaultSpeed) / distancePlayerEnemy;
		}
		else
		{
			enemy.speed.x = 0;
			enemy.speed.y = enemyDefaultSpeed;
		}
		
		enemy.rect.x += (int)(enemy.speed.x * frameDelay / 2);
		enemy.rect.y += (int)(enemy.speed.y * frameDelay / 2);



		/////////////////////////////

		switch (checkWallCollision(player.rect))
		{
		case TOP:
			player.rect.y = 0;
			break;
		case TOPR:
			player.rect.y = 0;
			player.rect.x = windowSize.w - player.rect.w;
			break;
		case RIGHT:
			player.rect.x = windowSize.w - player.rect.w;
			break;
		case BOTTOMR:
			player.rect.y = windowSize.h - player.rect.h;
			player.rect.x = windowSize.w - player.rect.w;
			break;
		case BOTTOM:
			player.rect.y = windowSize.h - player.rect.h;
			break;
		case BOTTOML:
			player.rect.y = windowSize.h - player.rect.h;
			player.rect.x = 0;
			break;
		case LEFT:
			player.rect.x = 0;
			break;
		case TOPL:
			player.rect.y = 0;
			player.rect.x = 0;
			break;
		default:
			break;
		}

		/////////////////////////////

		SDL_RenderClear(renderer);
		
		for (int i = 0; i < 2; i++) { SDL_RenderCopy(renderer, gameBackground.texture, NULL, &gameBackground.rect[i]); }

		if (player.onScreen == true) { SDL_RenderCopy(renderer, player.texture, NULL, &player.rect); }
		if (enemy.onScreen == true) { SDL_RenderCopy(renderer, enemy.texture, NULL, &enemy.rect); }

		for (int i = 0; i < MAXBULLET; i++)
		{
			if (playerBullet.onScreen[i]) { SDL_RenderCopy(renderer, playerBullet.texture, NULL, &playerBullet.rect[i]); }
		}

		SDL_RenderPresent(renderer);

		////////////////////////////

		frameTime = SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
	}
}

void Game::Finalize()
{
	Mix_CloseAudio();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}