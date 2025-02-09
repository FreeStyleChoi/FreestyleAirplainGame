#include "Game.h"

void Game::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("2025", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.w, windowSize.h, NULL);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer) { SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); }
	if (TTF_Init() == -1) { exit(-1); }

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 4, 2048);
	inGameBGM = Mix_LoadMUS("./asset/inGameBGM.ogg");

	Mix_VolumeMusic(64);

	SDL_Surface* surface;

	// ingame background
	surface = IMG_Load("./asset/backGroundFinal.png");
	gameBackground.texture = SDL_CreateTextureFromSurface(renderer, surface);

	gameBackground.speed.x = 0;
	gameBackground.speed.y = 0.3;
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
	player.defaultSpeed.x = 0.7;
	player.defaultSpeed.y = 0.7;
	player.onScreen = true;
	player.life = 3;

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
	enemy.defaultSpeed.x = 0.8;
	enemy.defaultSpeed.y = 0.8;
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
	Mix_PlayMusic(inGameBGM, -1);
	while (isRunning)
	{
		frameStart = SDL_GetTicks64();

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
			case SDLK_UP:			
				player.speed.y = player.defaultSpeed.y * -1;
				break;
			case SDLK_a:
			case SDLK_LEFT:
				player.speed.x = player.defaultSpeed.x * -1;
				break;
			case SDLK_s:
			case SDLK_DOWN:
				player.speed.y = player.defaultSpeed.y;
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				player.speed.x = player.defaultSpeed.x;
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
			case SDLK_UP:
			case SDLK_s:
			case SDLK_DOWN:
				player.speed.y = 0;
				break;

			case SDLK_a:
			case SDLK_LEFT:
			case SDLK_d:
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
		player.rect.x += (int)(player.speed.x * frameDelay);
		player.rect.y += (int)(player.speed.y * frameDelay);


		// enemy
		float distancePlayerEnemy = (float)sqrt(pow(player.rect.x - enemy.rect.x, 2) + pow(player.rect.y - enemy.rect.y, 2));
		if (enemy.rect.y < player.rect.y)
		{
			enemy.speed.y = ((float)(player.rect.y - enemy.rect.y) * enemy.defaultSpeed.y) / distancePlayerEnemy;
			enemy.speed.x = ((float)(player.rect.x - enemy.rect.x) * enemy.defaultSpeed.x) / distancePlayerEnemy;
		}
		else
		{
			enemy.speed.x = 0;
			enemy.speed.y = enemy.defaultSpeed.y;
		}
		
		enemy.rect.x += (int)(enemy.speed.x * frameDelay);
		enemy.rect.y += (int)(enemy.speed.y * frameDelay);


		/////////////////////////////

		// player
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

		if (checkCollision(player.rect, enemy.rect))
		{
			if (player.life == 0) { printf("game over!\n"); }
			else
			{
				player.life--;
				printf("player life : %d\n", player.life);
			}

			// enemy

			enemy.rect.x = (windowSize.w - enemy.rect.w) / 2;
			enemy.rect.y = 0;
			enemy.speed.x = 0;
			enemy.speed.y = 0;
		}

		if (checkWallCollision(enemy.rect) != NONE)
		{
			enemy.rect.x = (windowSize.w - enemy.rect.w) / 2;
			enemy.rect.y = 0;
			enemy.speed.x = 0;
			enemy.speed.y = 0;
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

		// font

		char tLife[9] = { 'a' };
		sprintf(tLife, "LIFE %d", player.life);
		printTTF(tLife, 36, renderer, 255, 255, 255, 0, 10, 10);

		char tScore[27] = { 'a' };
		sprintf(tScore, "SCORE %d", 0);
		printTTF(tScore, 36, renderer, 255, 255, 255, 0, 10, 56);


		SDL_RenderPresent(renderer);

		////////////////////////////

		frameTime = SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
	}
}

void Game::Finalize()
{
	Mix_FreeMusic(inGameBGM);
	Mix_CloseAudio();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}