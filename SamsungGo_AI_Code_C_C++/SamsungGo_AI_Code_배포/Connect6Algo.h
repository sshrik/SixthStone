// Samsung Go Tournament Form C Connect6Algo Header (g++-4.8.3)

// <--------------- 이 Code를 수정하면  작동하지 않을 수 있습니다 ------------------>

#pragma once
extern int width, height;
extern int terminateAI;
extern unsigned s_time;

extern FILE *fp;


extern int isFree(int x, int y);
extern int showBoard(int x, int y);
extern void init();
extern void restart();
extern void mymove(int x[], int y[], int cnt = 2);
extern void opmove(int x[], int y[], int cnt = 2);
extern void block(int, int);
extern int setLine(char *fmt, ...);
extern void domymove(int x[], int y[], int cnt = 2);
extern void writeLog(char *log);

void myturn(int cnt = 2);
char info[];

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#pragma warning(disable:4996)

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

void display(char plate[][PLATE_MAX]);
int allWhoWin(char plate[][PLATE_MAX]);
int whoWin(char plate[][PLATE_MAX], cord2D* cord, int turn);
void put(char plate[][PLATE_MAX], cord2D cord, int what);
int canPut(char plate[][PLATE_MAX], cord2D cord, int what);
void changeTurn(int * turn);
void initPlate(char plate[][PLATE_MAX], int blockNum);

int getLength(cord2D a, cord2D b);

int getCandidate(char plate[][PLATE_MAX], int candidateWeight[][PLATE_MAX], cord2D *candCord, cord2D * before, int candLimit, int * weightList, int turn);
int getCandWeight(char plate[][PLATE_MAX], cord2D temp, int turn);
void addWeight(char plate[][PLATE_MAX], cord2D temp, int cordWeight[][PLATE_MAX], int dir, int addNum);
long long int calcWeight(int cordWeight[][PLATE_MAX]);
int getWinState(char plate[][PLATE_MAX], cord2D *cord, int turn);
int isWinState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir, int continum);
int isOutOfPlate(cord2D cord, int next, int dir);
void doWin(char plate[][PLATE_MAX], cord2D temp, int dir, cord2D * next, int turn);
void doSheild(char plate[][PLATE_MAX], cord2D temp, int dir, cord2D * next, int turn);
int isSpecialCase(char plate[][PLATE_MAX], cord2D temp, int dir, int turn);
void sixthStoneBot(char plate[][PLATE_MAX], cord2D *next, cord2D *before, int doNext, int * weightList, int turn);
void changeBlocking(char plate[][PLATE_MAX], char changePlate[][PLATE_MAX], int turn);

#endif
