#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#ifndef SIX_STONE

#define PLATE_MAX 19
#define EAST	1
#define WEST	2
#define SOUTH	3
#define NORTH	4

#define EAST_SOUTH	5
#define EAST_NORTH	6
#define WEST_SOUTH	7
#define WEST_NORTH	8

#define WHITE	'W'
#define BLACK	'B'
#define BLOCK	'C'
#define EMPTY	'E'

#define NO -1
#define YES 1

void display(char plate[][PLATE_MAX], int lenX, int lenY);
int whoWin(int plate[][PLATE_MAX], int lenX, int lenY, int X, int Y);
void put(int plate[][PLATE_MAX], int X, int Y, int what);
int canPut(int plate[][PLATE_MAX], int lenX, int lenY, int X, int Y, int what);
void changeTurn(int * turn);
int getStateDir(char* state, char plate[][PLATE_MAX], int stateLen, int dir, int stateValue);
int getStrDir(char* str, char plate[][PLATE_MAX], int x, int y, int stateLen, int dir);

#endif
