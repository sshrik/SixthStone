#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

/* << CORDINATE Standard >>
 * X = i , y = j, so standard is plate[x][y].
 */

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

typedef struct{
	int x;
	int y;
} cord2D;

void display(char plate[][PLATE_MAX], cord2D lenCord);
int whoWin(char plate[][PLATE_MAX], cord2D lenCord, cord2D* cord, int turn);
void put(char plate[][PLATE_MAX], cord2D cord, int what);
int canPut(char plate[][PLATE_MAX], cord2D lenCord, cord2D cord, int what);
void changeTurn(int * turn);
int getStateDir(char* state, char plate[][PLATE_MAX], int stateLen, int dir, int stateValue);
int getStrDir(char* str, char plate[][PLATE_MAX], int x, int y, int stateLen, int dir);
//int getStrDir(char* str, char plate[][PLATE_MAX], cord2D cord, int stateLen, int dir);
void initPlate(char plate[][PLATE_MAX]);

int getCandidate(char plate[][PLATE_MAX], cord2D *candCord, int turn);
int getWinState(char plate[][PLATE_MAX], cord2D *cord, int turn);
int isWinState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir);
int isOutOfPlate(cord2D cord, int next, int dir);
int getState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir);
void doWin(char plate[][PLATE_MAX], cord2D * cord , int dir, cord2D * next, int turn);
void getNext(char plate[][PLATE_MAX], cord2D *candCord, cord2D *next, int turn);
void sixthStoneBot(char plate[][PLATE_MAX], cord2D lenCord, cord2D *next, cord2D *before, int turn);


#endif
