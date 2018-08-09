#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
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

#define DEPTH_MAX 10
#define CAND_MAX 6
#define WINSTATE_MAX 16
#define WIN_WEIGHT 99999

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

int getCandidate(char plate[][PLATE_MAX], int candidateWeight[][PLATE_MAX], cord2D *candCord, cord2D * before, int candLimit, int * weightList, int turn);
int getCandWeight(char plate[][PLATE_MAX], cord2D temp, int turn);
void addWeight(char plate[][PLATE_MAX], cord2D temp, int cordWeight[][PLATE_MAX], int dir, int addNum);
void changeCandWeight(char plate[][PLATE_MAX], int candidateWeight[][PLATE_MAX], cord2D before, int * weightList, int turn);
int getLength(cord2D a, cord2D b);
long long int calcWeight(int cordWeight[][PLATE_MAX]);
int getWinState(char plate[][PLATE_MAX], cord2D *cord, int turn);
int isWinState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir, int continum);
int isOutOfPlate(cord2D cord, int next, int dir);
void doWin(char plate[][PLATE_MAX], cord2D temp , int dir, cord2D * next, int turn);
void doSheild(char plate[][PLATE_MAX], cord2D temp, int dir, cord2D * next, int turn);
int isSpecialCase(char plate[][PLATE_MAX], cord2D temp, int dir, int turn);
void sixthStoneBot(char plate[][PLATE_MAX], cord2D *next, cord2D *before, int doNext, int * weightList, int turn, int nowTurn);
void changeBlocking(char plate[][PLATE_MAX], char changePlate[][PLATE_MAX], int turn);
int minMax(char tempPlate[][PLATE_MAX], cord2D *next, cord2D cord, int depth, int alpha, int beta, bool isMaximizingPlayer, cord2D *before, int tempCandidateWeight[][PLATE_MAX], int *weightList, int turnInTree, int turnCount, int myRealTurn, bool isRoot);

int isSevenStone(char plate[][PLATE_MAX], cord2D cord, int turn);
int getWinningStateNumber(char plate[][PLATE_MAX], int continum, int turn);
int doGame(int * weightListW, int * weightListB);
#endif
