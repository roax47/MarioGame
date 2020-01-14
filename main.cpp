#define _USE_MATH_DEFINESFLOOR_W
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"SDL_FUNCTION.h"
#include"MARIO.h"
#include"ENVIROMENT.h"

extern "C" {
#ifdef BIT64
#include"./sdl64/include/SDL.h"
#include"./sdl64/include/SDL_main.h"
#else
#include"./sdl/include/SDL.h"
#include"./sdl/include/SDL_main.h"
#endif
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define MARIO_W 24
#define MARIO_H 32
#define MARIO_DEFAULT_Y 399
#define JUMP_VELO 220
#define JUMP_COIN 70
#define JUMP_H 140
#define MARIO_SPEED 200
#define BACK_X 320
#define BACK_Y 240
#define FLOOR_H 32
#define FLOOR_W 32
#define BLOCK_H 32
#define BLOCK_W 32
#define ENEMY_H 32
#define ENEMY_W 32
#define ENEMY_SPEED 30
#define ENEMY_ROUTE 100
#define ENEMY_Y_DEAD 1000




// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc, block_count, floor_count, enemy_count, background_w, timelevel,meta_x,meta_y,level;
	level = 1;
	timelevel = 0;
	background_w = 0;
	floor_count = 0;
	block_count = 0;
	enemy_count = 0;
	meta_x = 0;
	meta_y = 0;
	double delta, worldTime, fpsTimer, fps, distance, levelTime;
	FLOOR *floor;
	BLOCK *block,*block_def; 
	ENEMY *enemy,*enemy_def;


	
	SDL_Event event;
	SDL_Surface *screen,*charset,*background,*menu,*meta;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect sprst[2];
	SDL_Rect dprst[2];



	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe³noekranowy
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Mario - Dariusz Grynia 2016");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./images/cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	background = SDL_LoadBMP("./images/background111.bmp");
	if (background == NULL) {
		printf("SDL_LoadBMP(screen.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	menu = SDL_LoadBMP("./images/menu.bmp");
	if (menu == NULL) {
		printf("SDL_LoadBMP(menu.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	sprst[0].y = 0;
	sprst[0].w = 640;
	sprst[0].h = 480;
	sprst[1].x = 640;
	sprst[1].y = 0;
	sprst[1].w = 600;
	sprst[1].h = 480;
	dprst[0].x = 0;
	dprst[0].y = 0;
	dprst[1].x = 640;
	dprst[1].y = 0;



	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	MARIO.state = DEFAULT_RIGHT;
	MARIO.speedX = 0;
	MARIO.speedY = 0;
	MARIO.lifes = 3;

	coin.x = 0;
	coin.y = 0;
	coin.jump = false;
	coin.fall = false;
	coin.counter = 0;
	int iniY_coin = 0;

	bool jump = false;
	bool fall = false;
	bool right = false;
	bool left = false;
	bool stand = true;
	bool newgame = false;
	bool load = false;
	int iniY = 0;

	int temp_coin = 0;
	int state_temp = 0;
	unsigned int templifes = 0;
	double temptime = 0;
	int statesave = 0;
	double tempY = 0;
	double tempX = 0;

	loadlevel(background_w, timelevel, MARIO, coin,&enemy, &floor,&block, floor_count, block_count, enemy_count, meta_x, meta_y,level);

	enemy_def = (ENEMY*)malloc(enemy_count*sizeof(ENEMY));
	block_def = (BLOCK*)malloc(block_count*sizeof(BLOCK));
	CopyArray_ENEMY(enemy, enemy_def, enemy_count);
	CopyArray_BLOCK(block, block_def, block_count);

	t1 = SDL_GetTicks();
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	int temp = 0;

	while (!quit) {

		t2 = SDL_GetTicks();
		delta = (t2 - t1) *0.001;
		t1 = t2;
		levelTime = timelevel - worldTime;
		worldTime += delta;
		if (MARIO.lifes <= 0) MARIO.state = MENU;

		////////////////////////// POZYCJA BLOKÓW WZGLEDEM MARIO ////////////////////////////////

		for (int i = 0; i < block_count; i++)
		{
			if (MARIO.xPOS + MARIO_W / 2 <= block[i].x + 1) block[i].relation = LEFT_FROM;
			if (MARIO.xPOS - MARIO_W / 2 >= block[i].x + BLOCK_H - 1) block[i].relation = RIGHT_FROM;
			if (MARIO.yPOS + MARIO_H / 2 < block[i].y - BLOCK_W + 2 && MARIO.xPOS + MARIO_W / 2 > block[i].x + 1 && MARIO.xPOS - MARIO_W / 2 < block[i].x + BLOCK_H-1) block[i].relation = OVER;
			if (MARIO.yPOS - MARIO_H / 2 > block[i].y - 1 && MARIO.xPOS + MARIO_W / 2 > block[i].x + 1 && MARIO.xPOS - MARIO_W / 2 < block[i].x + BLOCK_H - 1) block[i].relation = UNDER;
		}


		//////////////////// WARUNKI BACKGROUND ///////////////////////

		if ((int)MARIO.xPOS >= SCREEN_WIDTH / 2)
		{
			sprst[0].x = (int)MARIO.xPOS - SCREEN_WIDTH / 2;
			sprst[1].x = (int)MARIO.xPOS - SCREEN_WIDTH / 2;
		}

		if ((int)MARIO.xPOS <= SCREEN_WIDTH / 2)
		{
			sprst[0].x = 0;
			sprst[1].x = 640;

		}

		if ((int)MARIO.xPOS >= background_w - (SCREEN_WIDTH / 2))
		{
			sprst[0].x = background_w - 640;

		}

		/////////////////// RYSOWANIE ////////////////////////////////
		SDL_FillRect(screen, NULL, 0x5C94FC);
		SDL_BlitSurface(background, &sprst[0], screen, &dprst[0]);
		SDL_BlitSurface(background, &sprst[1], screen, &dprst[1]);
		DrawFloor(screen, floor, MARIO.xPOS, floor_count, background_w);
		DrawMeta(screen, meta_x, meta_y, MARIO.xPOS, background_w);
		DrawBlock(screen, block, MARIO.xPOS, block_count, background_w);
		DrawEnemy(screen, enemy, MARIO.xPOS, enemy_count, background_w);
		DrawMario(screen, MARIO.xPOS, MARIO.yPOS, MARIO.state, background_w);

		if (coin.jump == true)
		{ 
			DrawCoin(screen, coin, MARIO, background_w);
			coin.y -= JUMP_VELO*delta;
			if (coin.y <= iniY_coin + JUMP_COIN)
			{
				coin.jump = false;
				coin.fall = true;
			}
		}

		if (coin.fall == true)
		{
			DrawCoin(screen, coin, MARIO, background_w);
			coin.y += JUMP_VELO*delta;
			if (coin.y >= SCREEN_HEIGHT+BLOCK_H)
			{
				coin.counter++;
				coin.fall = false;
				coin.y = ENEMY_Y_DEAD;
			}
		}


		fpsTimer += delta;
		if (fpsTimer > 0.5) 
		{
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		sprintf(text, "Time left:%.1lf       Lifes:%d         Coins:%i         Level:%d", levelTime, MARIO.lifes, coin.counter,level);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

		////////////////MENU///////////////////////
		

		if (MARIO.state == MENU )
		{
			DrawSurface(screen, menu, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		}


		///////////////////////////////////////////////////////////////

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);


		///////////// OBSLUGA ZAPISU,WCZYTYWANIA,NOWEJ GRY //////////////////////
		
		if (newgame == true)
		{
			tempY = 0;
			tempX = 0;
			level = 1;
			free(enemy);
			free(enemy_def);
			loadlevel(background_w, timelevel, MARIO, coin, &enemy, &floor, &block, floor_count, block_count, enemy_count, meta_x, meta_y, level);
			enemy_def = (ENEMY*)malloc(enemy_count*sizeof(ENEMY));
			block_def = (BLOCK*)malloc(block_count*sizeof(BLOCK));
			MARIO.lifes = 3;
			MARIO.state = DEFAULT_RIGHT;
			worldTime = 0;
			coin.counter = 0;
			newgame = false;
		}

		if (load == true && templifes != 0 && tempX != 0 && tempY != 0)
		{
			CopyArray_ENEMY(enemy_def, enemy, enemy_count);
			CopyArray_BLOCK(block_def, block, block_count);
			coin.counter = temp_coin;
			MARIO.lifes = templifes;
			worldTime = temptime;
			MARIO.xPOS = tempX;
			MARIO.yPOS = tempY;
			MARIO.state = statesave;
			load = false;
		}

		if (MARIO.xPOS >= meta_x)
		{
			tempY = 0;
			tempX = 0;
			worldTime = 0;
			level++;
			MARIO.xPOS--;
			if (level <= 3)
			{
				free(enemy);
				free(enemy_def);
				loadlevel(background_w, timelevel, MARIO, coin, &enemy, &floor, &block, floor_count, block_count, enemy_count, meta_x, meta_y, level);
				worldTime = 0;
				enemy_def = (ENEMY*)malloc(enemy_count*sizeof(ENEMY));
				block_def = (BLOCK*)malloc(block_count*sizeof(BLOCK));
			}
			if (level > 3) MARIO.state = MENU;
		}

		if (levelTime <= 0)
		{
			worldTime = 0;
			tempY = 0;
			tempX = 0;
			free(enemy);
			free(enemy_def);
			loadlevel(background_w, timelevel, MARIO, coin, &enemy, &floor, &block, floor_count, block_count, enemy_count, meta_x, meta_y, level);
			MARIO.lifes--;
			enemy_def = (ENEMY*)malloc(enemy_count*sizeof(ENEMY));
			block_def = (BLOCK*)malloc(block_count*sizeof(BLOCK));

		}
		/////////// STA£A GRAWITACJA ///////////////
		if (MARIO.yPOS <= MARIO_DEFAULT_Y - 1 && jump != true)
		{

			fall = true;

		}
		////////////OBS£UGA RUCHU MARIO //////////////////
		check_mario_enemy(MARIO, enemy, enemy_count,worldTime);
		check_mario_floor(MARIO, floor, floor_count, right, left, jump, fall,stand, delta, worldTime);
		check_mario_colission(MARIO, block, block_count, right, left, jump, fall, stand, coin,iniY_coin, delta);


		MARIO.xPOS += MARIO.speedX*delta;
		MARIO.yPOS += MARIO.speedY*delta;




		/////////////////OBS£UGA RUCHU PRZECIWNIKÓW //////////////////////////

		for (int j = 0; j < enemy_count; j++)
		{
			for (int i = 0; i < block_count; i++)
			{

				if (enemy[j].x + ENEMY_W / 2 < block[i].x - 1) block[i].relation = LEFT_FROM;
				if (enemy[j].x + ENEMY_W / 2 > block[i].x + BLOCK_H + 1) block[i].relation = RIGHT_FROM;

			}
		}

		check_enemy_floor(enemy, enemy_count, floor, floor_count, delta);
		check_enemy_colission(enemy, enemy_count, block, block_count, delta,background_w);

	
		
		for (int i = 0; i < enemy_count; i++)
		{
			if(enemy[i].y<SCREEN_HEIGHT) enemy[i].x += enemy[i].speed*delta;
		}

	
		////////////////////////////// OGRANICZENIE KWRAWEDZI EKRANU //////////////////////////////
		if (MARIO.xPOS >= background_w - MARIO_W / 2 - 4 && right == true)
		{
			MARIO.speedX = 0;
			MARIO.xPOS = background_w - MARIO_W / 2;

		}
		
		if (MARIO.xPOS <= MARIO_W / 2 + 5 && left == true)
		{
			MARIO.speedX = 0;
			MARIO.xPOS = MARIO_W / 2;

		}
		

		////////////////// STANY //////////////////////////

		if (right == true)
		{
			if (stand == true)
			{
				MARIO.state = RIGHT;
			}
			else
			{
				MARIO.state = JUMP_RIGHT;
			}
			
			
		}
		if (left == true)
		{
			if (stand == true)
			{
				MARIO.state = LEFT;
			}
			else
			{
				MARIO.state = JUMP_LEFT;
			}
		}
		if (jump == true)
		{
			
			stand = false;
			fall = false;
			if (MARIO.yPOS > MARIO_DEFAULT_Y + 3) 
			{
				jump = false;
			}
			else
			{
				if (MARIO.yPOS <= iniY - JUMP_H)
				{
					MARIO.speedY = 0;
					jump = false;
					fall = true;

				}
			}
		}
		if (fall==true)
		{
			stand = false;
			MARIO.speedY = JUMP_VELO;
			if (MARIO.yPOS >= MARIO_DEFAULT_Y-1)
			{
				MARIO.speedY = 0;
				fall = false;
				stand = true;
				MARIO.yPOS = MARIO_DEFAULT_Y;
				if(MARIO.state == JUMP_RIGHT ) MARIO.state = DEFAULT_RIGHT;
				if (MARIO.state == JUMP_LEFT) MARIO.state = DEFAULT_LEFT;
			}
		}
	
		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (MARIO.state != MENU)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if (event.key.keysym.sym == SDLK_RIGHT)
					{
						right = true;
						MARIO.speedX = MARIO_SPEED;
					}
					else if (event.key.keysym.sym == SDLK_LEFT)
					{
						left = true;
						MARIO.speedX = -MARIO_SPEED;

					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						state_temp = MARIO.state;
						if (state_temp == RIGHT || state_temp == DEFAULT_RIGHT) MARIO.state = JUMP_RIGHT;
						if (state_temp == LEFT || state_temp == DEFAULT_LEFT) MARIO.state = JUMP_LEFT;
						if (jump != true && fall!=true)
							{
								jump = true;
								iniY = MARIO.yPOS;
								MARIO.speedY = -JUMP_VELO;	
							}

					}
					else if (event.key.keysym.sym == SDLK_s)
					{
						if (jump == false && fall == false)
						{
							CopyArray_ENEMY(enemy, enemy_def, enemy_count);
							CopyArray_BLOCK(block, block_def, block_count);
							temp_coin = coin.counter;
							templifes = MARIO.lifes;
							temptime = worldTime;
							tempX = MARIO.xPOS;
							tempY = MARIO.yPOS;
							statesave = MARIO.state;
						}
					}
					else if (event.key.keysym.sym == SDLK_l)
					{
						load = true;
					}
					else if (event.key.keysym.sym == SDLK_n)
					{
						newgame = true;

					}
				}
				else
				{
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if (event.key.keysym.sym == SDLK_SPACE)
					{
						newgame = true;
					}
				}
				break;
			case SDL_KEYUP:
				if (MARIO.state != MENU)
				{
					MARIO.speedX = 0;
					right = false;
					left = false;
					load = false;
					newgame = false;
					if (MARIO.state == RIGHT)MARIO.state = DEFAULT_RIGHT;
					if (MARIO.state == LEFT ) MARIO.state = DEFAULT_LEFT;
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
		frames++;
	};
	
	// zwolnienie powierzchni
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_FreeSurface(menu);
	SDL_FreeSurface(background);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	free(block);
	free(block_def);
	free(floor);
	free(enemy);
	free(enemy_def);


	SDL_Quit();
	return 0;
};
