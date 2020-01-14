#pragma once
#include "enviroment.h"

#ifndef MARIO_H
#define MARIO_H


struct Mario
{
	int state;
	double xPOS;
	double yPOS;
	double speedX;
	double speedY;
	unsigned int lifes;
}MARIO;

enum STATE
{
	DEFAULT_RIGHT,
	DEFAULT_LEFT,
	LEFT,
	RIGHT,
	JUMP_LEFT,
	JUMP_RIGHT,
	MENU
};

struct COIN 
{
	double x;
	double y;
	bool jump;
	bool fall;
	int counter;
}coin;

void loadlevel(int &background_w, int &timelevel, Mario &MARIO, COIN &coin, ENEMY **enemy, FLOOR **floor, BLOCK **block, int  &floor_count, int  & block_count, int  & enemy_count, int  & meta_x, int  & meta_y,int &level);
void DrawMario(SDL_Surface *screen,int x,int y,int state,int background_w);
void check_mario_enemy(Mario &MARIO, ENEMY* enemy, int enemy_count,double &worldTime);
void check_mario_floor(Mario &MARIO, FLOOR* floor, int floor_count, bool &right, bool &left, bool &jump, bool &fall, bool &stand, double &delta, double &worldTime);
void check_mario_colission(Mario &MARIO,BLOCK* block, int block_count, bool &right, bool &left, bool &jump, bool &fall, bool &stand,COIN &coin, int &iniY_coin, double &delta);
void DrawCoin(SDL_Surface *screen, COIN &coin,Mario &MARIO,int background);

#endif