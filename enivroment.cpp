#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "SDL_FUNCTION.h"


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
#define MARIO_SPEED 250
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



enum  STATE
{
	DEFAULT_RIGHT,
	DEFAULT_LEFT,
	LEFT,
	RIGHT,
	JUMP_LEFT,
	JUMP_RIGHT
};

struct FLOOR
{
	int x;
	int exist;
};

struct BLOCK
{
	int x;
	int y;
	int type;
	int relation;
	int exist;
};

struct ENEMY
{
	double x;
	double y;
	double speed;
};

enum RELATION
{
	LEFT_FROM,
	RIGHT_FROM,
	UNDER,
	OVER

};
enum TYPE
{
	DEF,
	DESTROY,
	COINBLOCK
};

void DrawFloor(SDL_Surface *screen, FLOOR *floor,int mario_x,int floor_count,int background)
{
	
	for (int i = 0; i < floor_count; i++)
	{
		
		SDL_Surface *f;
		f = SDL_LoadBMP("./images/gnd_red_1.bmp");
		if (f == NULL) 
		{
			printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
			SDL_FreeSurface(screen);
			SDL_Quit();
		};
		if (floor[i].exist == 1)
		{
			if (mario_x >= SCREEN_WIDTH / 2 && mario_x <= background-SCREEN_WIDTH/2)
			{
				DrawSurface(screen, f, floor[i].x + FLOOR_W/2 - (mario_x - SCREEN_WIDTH / 2), SCREEN_HEIGHT - FLOOR_H/2);
				DrawSurface(screen, f, floor[i].x + FLOOR_W/2 - (mario_x - SCREEN_WIDTH / 2), SCREEN_HEIGHT - (FLOOR_H/2 * 3));
				SDL_FreeSurface(f);
			}
			else if(mario_x >= background - SCREEN_WIDTH / 2)
			{
				DrawSurface(screen, f, floor[i].x + FLOOR_W/2- background + SCREEN_WIDTH,SCREEN_HEIGHT - FLOOR_H / 2);
				DrawSurface(screen, f, floor[i].x + FLOOR_W / 2 - background +SCREEN_WIDTH, SCREEN_HEIGHT - (FLOOR_H / 2 * 3));
				SDL_FreeSurface(f);
			}
			else 
			{
				DrawSurface(screen, f, floor[i].x + FLOOR_W / 2, SCREEN_HEIGHT - FLOOR_H / 2);
				DrawSurface(screen, f, floor[i].x + FLOOR_W / 2, SCREEN_HEIGHT - (FLOOR_H / 2 * 3));
				SDL_FreeSurface(f);
			}
		}
		else
		{
			SDL_FreeSurface(f);
		}
	}

}

void DrawBlock(SDL_Surface *screen, BLOCK *block, int mario_x, int block_count, int background)
{
	for (int i = 0; i < block_count; i++)
	{
		if (block[i].exist == 1)
		{
			SDL_Surface *b;
			if (block[i].type == DEF)b = SDL_LoadBMP("./images/blockq_used.bmp");
			if (block[i].type == DESTROY)b = SDL_LoadBMP("./images/brickred.bmp");
			if (block[i].type == COINBLOCK)b = SDL_LoadBMP("./images/blockq1_0.bmp");

			if (b == NULL)
			{
				printf("SDL_LoadBMP(blockq_used.bmp) error: %s\n", SDL_GetError());
				SDL_FreeSurface(screen);
				SDL_Quit();
			};
			SDL_SetColorKey(b, true, 0xFF00FF);
			if (mario_x >= SCREEN_WIDTH / 2 && mario_x <= background - SCREEN_WIDTH / 2)
			{
				DrawSurface(screen, b, block[i].x + BLOCK_W / 2 - (mario_x - SCREEN_WIDTH / 2), -BLOCK_H / 2 + block[i].y);
				SDL_FreeSurface(b);
			}
			else if (mario_x >= background - SCREEN_WIDTH / 2)
			{
				DrawSurface(screen, b, block[i].x + BLOCK_W / 2 - background + SCREEN_WIDTH, -BLOCK_H / 2 + block[i].y);
				SDL_FreeSurface(b);
			}
			else
			{
				DrawSurface(screen, b, block[i].x + BLOCK_W / 2, -BLOCK_H / 2 + block[i].y);
				SDL_FreeSurface(b);
			}
		}
	}

}

void DrawEnemy(SDL_Surface *screen, ENEMY *enemy, int mario_x, int enemy_count, int background)
{
	
	for (int i = 0; i < enemy_count; i++)
	{
		int temp = enemy[i].x*0.1;
		SDL_Surface *e;
		if(temp % 2 == 0) e = SDL_LoadBMP("./images/goombas_0.bmp");
		if (temp % 2 == 1) e = SDL_LoadBMP("./images/goombas_1.bmp");
		if (e == NULL)
		{
			printf("SDL_LoadBMP(goombas_1.bmp) error: %s\n", SDL_GetError());
			SDL_FreeSurface(screen);
			SDL_Quit();
		};
		SDL_SetColorKey(e, true, 0xFF00FF);
		if (mario_x >= SCREEN_WIDTH / 2 && mario_x <= background - SCREEN_WIDTH / 2)
		{
			DrawSurface(screen, e, enemy[i].x + ENEMY_W / 2 - (mario_x - SCREEN_WIDTH / 2), -ENEMY_H / 2+enemy[i].y);
			SDL_FreeSurface(e);
		}
		else if (mario_x >= background - SCREEN_WIDTH / 2)
		{
			DrawSurface(screen, e, enemy[i].x + ENEMY_W / 2 - background + SCREEN_WIDTH, -ENEMY_H / 2+ enemy[i].y);
			SDL_FreeSurface(e);
		}
		else
		{
			DrawSurface(screen, e, enemy[i].x + ENEMY_W / 2, -ENEMY_H / 2 +enemy[i].y);
			SDL_FreeSurface(e);
		}
	}

}

void CopyArray_ENEMY(ENEMY *enemy, ENEMY *enemy_def,int enemy_count)
{
	for (int i = 0; i < enemy_count; i++)
	{
		enemy_def[i].x = enemy[i].x;
		enemy_def[i].y = enemy[i].y;
		enemy_def[i].speed = enemy[i].speed;
	}
}

void CopyArray_BLOCK(BLOCK *block, BLOCK *block_def, int block_count)
{
	for (int i = 0; i < block_count; i++)
	{
		block_def[i].x = block[i].x;
		block_def[i].y = block[i].y;
		block_def[i].exist = block[i].exist;
	}
}

void check_enemy_floor(ENEMY *enemy, int enemy_count, FLOOR* floor, int floor_count, double &delta)
{
	for (int k = 0; k < enemy_count; k++)
	{
		for (int i = 0; i < floor_count; i++)
		{
			if (floor[i].exist == 0)
			{
				int j = i;
				while (floor[i + 1].exist == 0)
				{
					i++;
				}
				if (enemy[k].x > floor[j].x-FLOOR_W/2+ENEMY_W/2 && enemy[k].x < floor[i + 1].x - ENEMY_W  + 1)
				{
					enemy[k].y += (int)JUMP_VELO*delta;
					if (enemy[k].y > SCREEN_HEIGHT + 3)
					{
						enemy[k].y = ENEMY_Y_DEAD;

					}

				}
			}
		}
	}
}

void check_enemy_colission(ENEMY *enemy, int enemy_count, BLOCK* block, int block_count, double &delta,int background)
{
	for (int j = 0; j < enemy_count; j++)
	{
		for (int i = 0; i < block_count; i++)
		{
			if (enemy[j].x <= 2) enemy[j].speed = ENEMY_SPEED;
			if (enemy[j].x + ENEMY_W >= background-1) enemy[j].speed = -ENEMY_SPEED;

			if (block[i].relation == LEFT_FROM && enemy[j].x+ENEMY_W  >= block[i].x -1 && enemy[j].y - ENEMY_H / 2 >= block[i].y - BLOCK_H + 1 && enemy[j].y - ENEMY_H / 2 <= block[i].y - 1)
			{
				enemy[j].speed = -ENEMY_SPEED;

			}
			if (block[i].relation == RIGHT_FROM && enemy[j].x <= block[i].x +BLOCK_W +1 && enemy[j].y - ENEMY_H / 2 >= block[i].y - BLOCK_H + 1 && enemy[j].y - ENEMY_H / 2 <= block[i].y - 1)
			{
				enemy[j].speed = ENEMY_SPEED;
			}
		}
	}
}

void DrawMeta(SDL_Surface *screen,  int meta_x, int meta_y, int mario_x, int background)
{


	SDL_Surface *meta;
	meta = SDL_LoadBMP("./images/meta.bmp");
	if (meta == NULL) 
	{
		printf("SDL_LoadBMP(screen.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_Quit();
	};
	if (meta_x >= SCREEN_WIDTH / 2 && mario_x <= background - SCREEN_WIDTH / 2)
	{
		DrawSurface(screen, meta, meta_x + meta->w / 2 - (mario_x - SCREEN_WIDTH / 2), -meta->h / 2 + meta_y);
		SDL_FreeSurface(meta);
	}
	else if (mario_x >= background - SCREEN_WIDTH / 2)
	{
		DrawSurface(screen, meta, meta_x + meta->w / 2 - background + SCREEN_WIDTH, -meta->h / 2 + meta_y);
		SDL_FreeSurface(meta);
	}
	else
	{
		DrawSurface(screen, meta, meta_x + meta->w / 2, -meta->h / 2 + meta_y);
		SDL_FreeSurface(meta);
	}
}
