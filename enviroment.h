#pragma once

#ifndef ENVIROMENT_H
#define ENVIROMENT_H


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

void DrawFloor(SDL_Surface *screen, FLOOR *floor,int mario_x,int floor_count, int background);
void DrawMeta(SDL_Surface *screen, int meta_x, int meta_y, int mario_x, int background);
void DrawBlock(SDL_Surface *screen, BLOCK *block, int mario_x, int block_count, int background);
void DrawEnemy(SDL_Surface *screen, ENEMY *enemy, int mario_x, int enemy_count, int background);
void CopyArray_ENEMY(ENEMY *enemy, ENEMY *enemy_def,int enemy_count);
void CopyArray_BLOCK(BLOCK *block, BLOCK *block_def, int block_count);
void check_enemy_floor(ENEMY *enemy, int enemy_count, FLOOR* floor, int floor_count, double &delta);
void check_enemy_colission(ENEMY *enemy,int enemy_count, BLOCK* block, int block_count,double &delta, int background);


#endif