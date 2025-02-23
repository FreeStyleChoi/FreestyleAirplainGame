#include "Game.h"

////////////////////////////////////////////////////////
// TODO: 게임오버 시 이벤트 처리
////////////////////////////////////////////////////////



void Game::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("2025", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.w, windowSize.h, NULL);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer) { SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); }
	if (TTF_Init() == -1) { exit(-1); }

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 4, 2048);

	int numJoysticks = SDL_NumJoysticks();
	joystick = SDL_JoystickOpen(0);
	const char* asdf = SDL_JoystickName(joystick);
}


void Game::MenuScreen()
{
	bool isRunning = true;
	bool Exit = false;
	SDL_Surface* surface = IMG_Load("./asset/mainBackground.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect rect{ 0, 0, windowSize.w, windowSize.h };

	event.key.keysym.sym = SDLK_UNKNOWN;

	Mix_Music* menuBGM = Mix_LoadMUS("./asset/menuBGM.ogg");

	Mix_PlayMusic(menuBGM, -1);
	int asdf=0;
	while (isRunning)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			Exit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				isRunning = false;
				Exit = true;
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				if (FPS > 30)
					FPS -= 10;
				break;
			case SDLK_RIGHT:
				if (FPS < 250)
					FPS += 10;
				break;
			default:

				isRunning = false;
				event.key.keysym.sym = SDLK_UNKNOWN;
				break;
			}
			break;
		case SDL_JOYBUTTONDOWN:
			isRunning = false;
			event.key.keysym.sym = SDLK_UNKNOWN;
			break;
		default:
			break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &rect);

		char tFps[8] = { '\0' };
		sprintf(tFps, "FPS %d", FPS);
		printTTF(tFps, 36, renderer, 255, 255, 255, 0, 10, 10);
		frameDelay = 1000 / FPS;

		SDL_RenderPresent(renderer);
	}

	Mix_PauseMusic();
	Mix_FreeMusic(menuBGM);
	SDL_DestroyTexture(texture);

	if (Exit)
	{
		SDL_CloseAudio();
		SDL_JoystickClose(joystick);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyTexture(texture);
		SDL_Quit();
		exit(0);
	}
	else
		return;
}


void Game::Setting()
{
	isRunning = true;

	// music & sound effect
	inGameBGM = Mix_LoadMUS("./asset/inGameBGM.ogg");
	bulletShootSoundEffect = Mix_LoadWAV("./asset/shooting.wav");
	bulletHitSoundEffect = Mix_LoadWAV("./asset/coin.wav");
	gameoverEffect = Mix_LoadWAV("./asset/gameover.wav");

	Mix_VolumeMusic(64);
	Mix_VolumeChunk(bulletShootSoundEffect, 32);

	// ingame background

	SDL_Surface* surface;

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
	surface = IMG_Load("./asset/damagedEffect.png");
	damagedEffect = SDL_CreateTextureFromSurface(renderer, surface);

	player.rect.w = 46;
	player.rect.h = 43;
	player.rect.x = windowSize.w / 2 - (player.rect.w / 2);
	player.rect.y = windowSize.h - player.rect.h;
	player.speed.x = 0;
	player.speed.y = 0;
	player.defaultSpeed.x = 0.7;
	player.defaultSpeed.y = 0.7;
	player.onScreen = true;
	player.life = 10;
	score = 0;

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
	surface = IMG_Load("./asset/weapon.png");
	playerBullet.texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	for (int i = 0; i < MAXBULLET; i++)
	{
		playerBullet.rect[i].w = 16;
		playerBullet.rect[i].h = 16;
		playerBullet.rect[i].x = player.rect.x + (player.rect.w / 2) - playerBullet.rect[i].w / 2;
		playerBullet.rect[i].y = player.rect.y + playerBullet.rect[i].h;
		playerBullet.speed.x = 0;
		playerBullet.speed.y = 0;
		playerBullet.defaultSpeed.x = 0;
		playerBullet.defaultSpeed.y = -1.4;
		playerBullet.onScreen[i] = false;
	}

	SDL_FreeSurface(surface);

	// enemy bullet
	surface = IMG_Load("./asset/enemy_weapon.png");
	enemyBullet.texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	for (int i = 0; i < MAXBULLET; i++)
	{
		enemyBullet.rect[i].w = 16;
		enemyBullet.rect[i].h = 16;
		enemyBullet.rect[i].x = enemy.rect.x + ((enemy.rect.w / 2) - (enemyBullet.rect[i].w / 2));
		enemyBullet.rect[i].y = enemy.rect.y - enemyBullet.rect[i].h;
		enemyBullet.speed.x = 0;
		enemyBullet.speed.y = 0;
		enemyBullet.defaultSpeed.x = 0;
		enemyBullet.defaultSpeed.y = 1.4;
		enemyBullet.onScreen[i] = false;
	}

	SDL_FreeSurface(surface);

	// rendom number
	srand((unsigned int)time(NULL) * rand());

	// play music
	Mix_PlayMusic(inGameBGM, -1);
}

void Game::Update()
{
	unsigned int frameCount = 1;

	bool isDamaged = false;
	Uint32 damagedT = 0;

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

			case SDLK_LSHIFT:
				player.defaultSpeed = { 0.3f, 0.3f };
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

			case SDLK_SPACE:
				for (int i = 0; i < MAXBULLET; i++)
				{
					if (playerBullet.onScreen[i] == false)
					{
						playerBullet.onScreen[i] = true;
						Mix_PlayChannel(-1, bulletShootSoundEffect, 0);
						if (score > 0) { score--; }
						break;
					}
				}
				break;

			case SDLK_LSHIFT:
				player.defaultSpeed = { 0.7f, 0.7f };
				break;

			default:
				break;
			}

			break;

		case SDL_JOYAXISMOTION:
			if (event.jaxis.axis == 0) // move X
			{
				if (event.jaxis.value > 0)
					player.speed.x = player.defaultSpeed.x;

				else if (event.jaxis.value < 0)
					player.speed.x = player.defaultSpeed.x * -1;

				else
					player.speed.x = 0;
			}
			if (event.jaxis.axis == 1) // move Y
			{
				if (event.jaxis.value > 0)
					player.speed.y = player.defaultSpeed.y;

				else if (event.jaxis.value < 0)
					player.speed.y = player.defaultSpeed.y * -1;

				else
					player.speed.y = 0;
			}
			break;
			
		case SDL_JOYBUTTONDOWN:
			if (event.jbutton.button != 7)
			{
				for (int i = 0; i < MAXBULLET; i++)
				{
					if (playerBullet.onScreen[i] == false)
					{
						playerBullet.onScreen[i] = true;
						Mix_PlayChannel(-1, bulletShootSoundEffect, 0);
						if (score > 0) { score--; }
						break;
					}
				}
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
			if (!player.life == 0)
			{
				player.life--;
				isDamaged = true;
			}

			// enemy

			enemy.rect.x = rand() % (windowSize.w - enemy.rect.w);
			enemy.rect.y = 0;
			enemy.speed.x = 0;
			enemy.speed.y = 0;
		}


		if (checkWallCollision(enemy.rect) != NONE)
		{
			enemy.rect.x = rand() % (windowSize.w - enemy.rect.w);
			enemy.rect.y = 0;
			enemy.speed.x = 0;
			enemy.speed.y = 0;
		}

		// about bullet

		for (int i = 0; i < MAXBULLET; i++)
		{

			// player bullet
			if (playerBullet.onScreen[i])
			{
				// enemy - player bullet collision

				if (checkCollision(enemy.rect, playerBullet.rect[i]))
				{
					enemy.rect.x = rand() % (windowSize.w - enemy.rect.w);
					enemy.rect.y = 0;
					score += 10;
					playerBullet.onScreen[i] = false;
					Mix_PlayChannel(-1, bulletHitSoundEffect, 0);
				}

				// player bullet - wall

				else if (checkWallCollision(playerBullet.rect[i]) != NONE)
				{
					playerBullet.onScreen[i] = false;
				}

				playerBullet.speed = playerBullet.defaultSpeed;

				playerBullet.rect[i].x += playerBullet.speed.x * frameDelay;
				playerBullet.rect[i].y += playerBullet.speed.y * frameDelay;
			}
			else
			{
				playerBullet.rect[i].x = player.rect.x + (player.rect.w / 2) - playerBullet.rect[i].w / 2;
				playerBullet.rect[i].y = player.rect.y + playerBullet.rect[i].h;
			}
			
			///////////////////////////////////////////////

			// enemy bullet

			if (checkCollision(player.rect, enemyBullet.rect[i]))
			{
				enemyBullet.onScreen[i] = false;
				if (!player.life == 0)
				{
					player.life--;
					isDamaged = true;
				}
			}

			else if (checkWallCollision(enemyBullet.rect[i]) != NONE)
			{
				enemyBullet.onScreen[i] = false;
			}

			if (!enemyBullet.onScreen[i])
			{
				enemyBullet.rect[i].x = enemy.rect.x + ((enemy.rect.w / 2) - (enemyBullet.rect[i].w / 2));
				enemyBullet.rect[i].y = enemy.rect.y - enemyBullet.rect[i].h;
			}

			enemyBullet.speed = enemyBullet.defaultSpeed;
			enemyBullet.rect[i].x += enemyBullet.speed.x * frameDelay;
			enemyBullet.rect[i].y += enemyBullet.speed.y * frameDelay;

			
		}

		// lunch enemy bullet
		for (int i = 0; i < MAXBULLET; i++)
		{
			if (enemyBullet.onScreen[i] == false && (frameCount * frameDelay) % 500 == 0)
			{
				enemyBullet.onScreen[i] = true;
				Mix_PlayChannel(-1, bulletShootSoundEffect, 0);
				break;
			}
		}

		///////////////////////////// 

		SDL_RenderClear(renderer);
		
		for (int i = 0; i < 2; i++) { SDL_RenderCopy(renderer, gameBackground.texture, NULL, &gameBackground.rect[i]); }

		if (player.onScreen == true)
		{
			if (isDamaged == true)
			{
				doingDamageE = true;
				damagedT = frameCount;
				isDamaged = false;
			}

			if (doingDamageE == true && (frameCount * frameDelay) - (damagedT * frameDelay) >= 500)
			{
				doingDamageE = false;
				damagedT = 0;
			}

			if (doingDamageE == true)
				SDL_RenderCopy(renderer, damagedEffect, NULL, &player.rect);
			else
				SDL_RenderCopy(renderer, player.texture, NULL, &player.rect);
		}
		if (enemy.onScreen == true) { SDL_RenderCopy(renderer, enemy.texture, NULL, &enemy.rect); }

		for (int i = 0; i < MAXBULLET; i++)
		{
			if (playerBullet.onScreen[i]) { SDL_RenderCopy(renderer, playerBullet.texture, NULL, &playerBullet.rect[i]); }
			if (enemyBullet.onScreen[i]) { SDL_RenderCopy(renderer, enemyBullet.texture, NULL, &enemyBullet.rect[i]); }
		}

		// font

		char tFps[8] = { '\0' };
		sprintf(tFps, "FPS %d", FPS);
		printTTF(tFps, 36, renderer, 255, 255, 255, 0, 10, 10);

		char tLife[9] = { '\0' };
		sprintf(tLife, "LIFE %d", player.life);
		printTTF(tLife, 36, renderer, 255, 255, 255, 0, 10, 56);

		char tScore[27] = { '\0' };
		sprintf(tScore, "SCORE %d", score);
		printTTF(tScore, 36, renderer, 255, 255, 255, 0, 10, 102);

		if (player.life <= 0)
		{
			char tGameover[10] = { "GAME OVER" };
			printTTF(tGameover, 60, renderer, 255, 0, 0, 0, (windowSize.w / 2) - ((60 / 2) * (9 / 2)), (windowSize.h / 2) - (60 / 2));
			Mix_PauseMusic();
			Mix_PlayChannel(-1, gameoverEffect, 0);
			isRunning = false;
			restart = true;
		}
		else
			restart = false;


		SDL_RenderPresent(renderer);

		////////////////////////////

		frameTime = SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);

		frameCount++;

		if (player.life <= 0)
		{
			doingDamageE = false;

			Uint32 GOframeTime;
			Uint32 GOframeStart;
			for (int i = 1; i < 2000 / frameDelay; i++)
			{
				GOframeStart = SDL_GetTicks64();

				if (i * frameDelay % 2001 == 2000)
				{
					return; // 게임오버 효과음을 위한 1.5초 대기
				}

				// 게임오버 이펙트 시 이벤트 처리 작업
				SDL_PollEvent(&event);
				switch (event.type)
				{
				case SDL_QUIT:
					restart = false;
					return;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						restart = false;
						return;
						break;
					default:
						break;
					}
				default:
					break;
				}

				GOframeTime = SDL_GetTicks64() - GOframeStart;
				if (frameDelay > GOframeTime) SDL_Delay(frameDelay - GOframeTime);
			}
		}

	}
}

void Game::Finalize()
{
	SDL_DestroyTexture(playerBullet.texture);
	SDL_DestroyTexture(enemyBullet.texture);
	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(enemy.texture);
	SDL_DestroyTexture(damagedEffect);
	SDL_DestroyTexture(gameBackground.texture);


	Mix_FreeMusic(inGameBGM);
	Mix_FreeChunk(bulletShootSoundEffect);
	Mix_FreeChunk(bulletHitSoundEffect);
	Mix_FreeChunk(gameoverEffect);
	Mix_CloseAudio();

	SDL_JoystickClose(joystick);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}