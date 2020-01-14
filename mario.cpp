#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "SDL_FUNCTION.h"
#include "enviroment.h"

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
#define JUMP_VELO 200
#define JUMP_COIN 70
#define JUMP_H 140
#define MARIO_SPEED 220
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
	JUMP_RIGHT,
	MENU
};

struct Mario
{
	int state;
	double xPOS;
	double yPOS;
	double speedX;
	double speedY;
	unsigned int lifes;
};

struct COIN
{
	double x;
	double y;
	bool jump;
	bool fall;
	int counter;
};


void DrawMario(SDL_Surface *screen, int x, int y, int state,int background_w)
{
	int temp = x*0.04;
	if (x >= SCREEN_WIDTH / 2 && x <= background_w - (SCREEN_WIDTH / 2)) x = SCREEN_WIDTH / 2;
	if (x >= background_w -(SCREEN_WIDTH / 2)) x -= (background_w - SCREEN_WIDTH);
	
		if (state == RIGHT)
		{
			
			if (temp % 3 == 0)
			{
				SDL_Surface *mariomove1;
				mariomove1= SDL_LoadBMP("./images/mario/mario_move0.bmp");
				if (mariomove1 == NULL) {
					printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
					SDL_FreeSurface(screen);
					SDL_Quit();
				};
				SDL_SetColorKey(mariomove1, true, 0xFF00FF);
				DrawSurface(screen, mariomove1, x, y);
				SDL_FreeSurface(mariomove1);
			}
			else if (temp % 3 == 1)
			{
				SDL_Surface *mariomove2;
				mariomove2 = SDL_LoadBMP("./images/mario/mario_move1.bmp");
				if (mariomove2 == NULL) {
					printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
					SDL_FreeSurface(screen);
					SDL_Quit();
				};
				SDL_SetColorKey(mariomove2, true, 0xFF00FF);
				DrawSurface(screen, mariomove2, x, y);
				SDL_FreeSurface(mariomove2);

			}
			else
			{
				SDL_Surface *mariomove3;
				mariomove3 = SDL_LoadBMP("./images/mario/mario_move2.bmp");
				if (mariomove3 == NULL) {
					printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
					SDL_FreeSurface(screen);
					SDL_Quit();
				};
				SDL_SetColorKey(mariomove3, true, 0xFF00FF);
				DrawSurface(screen, mariomove3, x, y);
				SDL_FreeSurface(mariomove3);
			}
				
			
		
		}
		if (state == LEFT)
		{
			if (temp % 3 == 0)
			{
				SDL_Surface *mariomove1_left;
				mariomove1_left = SDL_LoadBMP("./images/mario/mario_move0_left.bmp");
				if (mariomove1_left == NULL) {
					printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
					SDL_FreeSurface(screen);
					SDL_Quit();
				};
				SDL_SetColorKey(mariomove1_left, true, 0xFF00FF);
				DrawSurface(screen, mariomove1_left, x, y);
				SDL_FreeSurface(mariomove1_left);
			}
			else if (temp % 3 == 1)
			{
				SDL_Surface *mariomove2_left;
				mariomove2_left = SDL_LoadBMP("./images/mario/mario_move1_left.bmp");
				if (mariomove2_left == NULL) {
					printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
					SDL_FreeSurface(screen);
					SDL_Quit();
				};
				SDL_SetColorKey(mariomove2_left, true, 0xFF00FF);
				DrawSurface(screen, mariomove2_left, x, y);
				SDL_FreeSurface(mariomove2_left);

			}
			else
			{
				SDL_Surface *mariomove3_left;
				mariomove3_left = SDL_LoadBMP("./images/mario/mario_move2_left.bmp");
				if (mariomove3_left == NULL) {
					printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
					SDL_FreeSurface(screen);
					SDL_Quit();
				};
				SDL_SetColorKey(mariomove3_left, true, 0xFF00FF);
				DrawSurface(screen, mariomove3_left, x, y);
				SDL_FreeSurface(mariomove3_left);
			}
		
		}
		if (state == JUMP_RIGHT)
		{
			
				SDL_Surface *mariojump;
				mariojump = SDL_LoadBMP("./images/mario/mario_jump.bmp");
				if (mariojump == NULL) {
					printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
					SDL_FreeSurface(screen);
					SDL_Quit();
				};
				SDL_SetColorKey(mariojump, true, 0xFF00FF);
				DrawSurface(screen, mariojump, x, y);
				SDL_FreeSurface(mariojump);
			
		}
		if (state == JUMP_LEFT)
		{

			SDL_Surface *mariojump;
			mariojump = SDL_LoadBMP("./images/mario/mario_jump_left.bmp");
			if (mariojump == NULL) {
				printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
				SDL_FreeSurface(screen);
				SDL_Quit();
			};
			SDL_SetColorKey(mariojump, true, 0xFF00FF);
			DrawSurface(screen, mariojump, x, y);
			SDL_FreeSurface(mariojump);

		}
		if (state == DEFAULT_RIGHT)
		{
			SDL_Surface *mariodef;
			mariodef = SDL_LoadBMP("./images/mario/mario.bmp");
			if (mariodef == NULL) {
				printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
				SDL_FreeSurface(screen);
				SDL_Quit();
			};
			SDL_SetColorKey(mariodef, true, 0xFF00FF);
			DrawSurface(screen, mariodef, x, y);
			SDL_FreeSurface(mariodef);
		}
		if (state == DEFAULT_LEFT)
		{
			SDL_Surface *mariodef_left;
			mariodef_left = SDL_LoadBMP("./images/mario/mario_left.bmp");
			if (mariodef_left == NULL) {
				printf("SDL_LoadBMP(mario.bmp) error: %s\n", SDL_GetError());
				SDL_FreeSurface(screen);
				SDL_Quit();
			};
			SDL_SetColorKey(mariodef_left, true, 0xFF00FF);
			DrawSurface(screen, mariodef_left, x, y);
			SDL_FreeSurface(mariodef_left);
		}
		
}

void check_mario_enemy(Mario &MARIO, ENEMY* enemy,int enemy_count, double &worldTime)
{
	for (int i = 0; i < enemy_count; i++)
	{
		if (MARIO.xPOS >= enemy[i].x - MARIO_W / 2 && MARIO.xPOS <= enemy[i].x + ENEMY_H + MARIO_W / 2 && MARIO.yPOS >= enemy[i].y - ENEMY_H-MARIO_H/2  && MARIO.yPOS + MARIO_H / 2 <= enemy[i].y + 2)
		{
			if (MARIO.state == JUMP_RIGHT) MARIO.state = DEFAULT_RIGHT;
			if (MARIO.state == JUMP_LEFT) MARIO.state = DEFAULT_LEFT;
			worldTime = 0;
			MARIO.lifes --;
			MARIO.xPOS = MARIO_W / 2;
			MARIO.yPOS = MARIO_DEFAULT_Y;
		}
	}
}

void check_mario_floor(Mario &MARIO, FLOOR* floor, int floor_count,bool &right, bool &left ,bool &jump, bool &fall, bool &stand,double &delta, double &worldTime)
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
			if (MARIO.xPOS > floor[j].x + MARIO_W / 2 - 10 && MARIO.xPOS < floor[i + 1].x - MARIO_W / 2 + 10 && jump == false && fall == false)
			{
				stand = false;
				MARIO.yPOS += (int)JUMP_VELO*delta;
				if (MARIO.yPOS > MARIO_DEFAULT_Y + 3)
				{
					MARIO.speedY = 0;
					if (MARIO.xPOS < floor[j].x + MARIO_W / 2 + 1)
					{
						MARIO.speedX = 0;
						MARIO.xPOS = floor[j].x + MARIO_W / 2;

					}
					if (MARIO.xPOS> floor[i + 1].x - MARIO_W / 2 - 1)
					{
						MARIO.speedX = 0;
						MARIO.xPOS = floor[i + 1].x - MARIO_W / 2;
					}
				}

			}
			if (MARIO.yPOS >= SCREEN_HEIGHT)
			{
				stand = true;
				if (MARIO.state == JUMP_RIGHT) MARIO.state = DEFAULT_RIGHT;
				if (MARIO.state == JUMP_LEFT) MARIO.state = DEFAULT_LEFT;
				worldTime = 0;
				MARIO.lifes --;
				MARIO.xPOS = MARIO_W/2;
				MARIO.yPOS = MARIO_DEFAULT_Y;
			}
		}
	}
}

void check_mario_colission(Mario &MARIO,BLOCK* block, int block_count, bool &right, bool &left, bool &jump, bool &fall,bool &stand, COIN &coin , int &iniY_coin,double &delta)
{
	for (int i = 0; i < block_count; i++)
	{
		if (block[i].exist == 1)
		{
			if (block[i].relation == LEFT_FROM && MARIO.xPOS + MARIO_W / 2 >= block[i].x - 1
				&& MARIO.yPOS + MARIO_H / 2 >= block[i].y - BLOCK_H + 1
				&& MARIO.yPOS - MARIO_H / 2 <= block[i].y - 1)
			{
				if (right == true)MARIO.speedX = 0;
				fall = true;

			}
			if (block[i].relation == RIGHT_FROM && MARIO.xPOS - MARIO_W / 2 <= block[i].x + BLOCK_W + 1
				&& MARIO.yPOS + MARIO_H / 2 >= block[i].y - BLOCK_H + 1
				&& MARIO.yPOS - MARIO_H / 2 <= block[i].y - 1)
			{
				if (left == true)MARIO.speedX = 0;
				fall = true;
			}
			if (block[i].relation == UNDER &&  MARIO.yPOS - MARIO_H / 2 <= block[i].y + 1)
			{
				jump = false;
				fall = true;
				if (block[i].type == DESTROY)
				{
					block[i].exist = 0;
				}
				if (block[i].type == COINBLOCK)
				{
					iniY_coin = block[i].y - BLOCK_H;
					coin.jump = true;
					coin.x = block[i].x;
					coin.y = block[i].y - BLOCK_H;
					block[i].exist = 0;
				}
			}
			if (block[i].relation == OVER &&  MARIO.yPOS + MARIO_H / 2 >= block[i].y - BLOCK_H - 1)
			{
				MARIO.yPOS = block[i].y - BLOCK_H - 1 - MARIO_H / 2;
				stand = true;
				if (MARIO.state == JUMP_RIGHT) MARIO.state = DEFAULT_RIGHT;
				if (MARIO.state == JUMP_LEFT) MARIO.state = DEFAULT_LEFT;
				fall = false;
			}
		}
	}
}

void DrawCoin(SDL_Surface *screen, COIN &coin, Mario &MARIO, int background)
{

		SDL_Surface *c;
		c = SDL_LoadBMP("./images/coin_use00.bmp");
		

		if (c == NULL) {
			printf("SDL_LoadBMP(coin_use00.bmp) error: %s\n", SDL_GetError());
			SDL_FreeSurface(c);
			SDL_Quit();
		};
		SDL_SetColorKey(c, true, 0xFF00FF);
		if (MARIO.xPOS >= SCREEN_WIDTH / 2 && MARIO.xPOS <= background - SCREEN_WIDTH / 2)
		{
			DrawSurface(screen, c, coin.x + BLOCK_H / 2 - (MARIO.xPOS - SCREEN_WIDTH / 2), -BLOCK_H / 2 + coin.y);
			SDL_FreeSurface(c);
		}
		else if (MARIO.xPOS >= background - SCREEN_WIDTH / 2)
		{
			DrawSurface(screen, c, coin.x + BLOCK_H / 2 - background + SCREEN_WIDTH, -BLOCK_H / 2 + coin.y);
			SDL_FreeSurface(c);
		}
		else
		{
			DrawSurface(screen, c, coin.x +BLOCK_H/2, -BLOCK_H / 2 + coin.y);
			SDL_FreeSurface(c);
		}
}

void loadlevel(int &background_w, int &timelevel, Mario &MARIO, COIN &coin, ENEMY **enemy, FLOOR **floor,BLOCK **block, int  &floor_count, int  & block_count, int  & enemy_count, int  & meta_x, int  & meta_y,int &level)
{
	char filename[64];
	sprintf(filename, "levels/etap%d.txt", level);
	FILE* file;
	file = fopen(filename, "rb");
	if (fopen == NULL)
	{
		printf("File failed to open");
		exit(1);
	}
	fscanf(file,"%d %lf %lf ", &background_w,&MARIO.xPOS,&MARIO.yPOS);
	if (MARIO.xPOS<0 || MARIO.xPOS>background_w / FLOOR_H + 1 || MARIO.yPOS>SCREEN_HEIGHT)  return;
	MARIO.xPOS = MARIO.xPOS*FLOOR_W + MARIO_W / 2;
	MARIO.yPOS = MARIO.yPOS*FLOOR_H - MARIO_H / 2;
	fscanf(file, "%d %d %d %d ", &timelevel, &block_count, &enemy_count,&meta_x);
	if (meta_x<0 || meta_x>background_w / FLOOR_H + 1 || meta_y>SCREEN_HEIGHT)  return;
	floor_count = (background_w / FLOOR_W) + 1;
	meta_x *= FLOOR_W;
	meta_y = SCREEN_HEIGHT;
	*floor = (FLOOR*)malloc(floor_count*sizeof(FLOOR));
	*block = (BLOCK*)malloc(floor_count*sizeof(BLOCK));
	*enemy = (ENEMY*)malloc(floor_count*sizeof(ENEMY));
	
	for (int i = 0; i < floor_count; i++)
	{
		fscanf(file, "%d ", &(*floor)[i].exist);
		(*floor)[i].x = FLOOR_H*i;
		if ((*floor)[0].exist == 0)(*floor)[0].exist = 1;
	}

	for (int i = 0; i < block_count; i++)
	{
		fscanf(file, "%d %d %d ", &(*block)[i].x, &(*block)[i].y, &(*block)[i].type);
		
		(*block)[i].x = (*block)[i].x*FLOOR_W;
		(*block)[i].y = (*block)[i].y*FLOOR_H;
		(*block)[i].exist = 1;
		if ((*block)[i].x == 0)(*block)[i].exist = 0;
	}
	for (int i = 0; i < enemy_count; i++)
	{
		fscanf(file, "%lf %lf ", &(*enemy)[i].x, &(*enemy)[i].y);
		if ((*enemy)[i].x == 0)(*enemy)[i].x = 1;
		(*enemy)[i].x = (*enemy)[i].x*FLOOR_W;
		(*enemy)[i].y = (*enemy)[i].y*FLOOR_H;
		(*enemy)[i].speed = ENEMY_SPEED;
	}
	
	fclose(file);
}
