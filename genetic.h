#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#ifndef SIX_GEN

#define GEN_LENGTH 20
#define GEN_MAX 1000
#define PARENT_MAX 4
#define CHILD_NUM 2
#define GENERATION_MAX PARENT_MAX * PARENT_MAX * CHILD_NUM
#define GAME_MAX 10

#define MUT_PER 70

void initGen(int gen[][GEN_LENGTH]);
void makeChild(int par[][GEN_LENGTH], int child[][GEN_LENGTH]);
int calcProfit(int * child, int * dst);
void makeMutate(int * gen);
void choosePar(int gen[][GEN_LENGTH], int par[][GEN_LENGTH]);
int findProfit(int child[][GEN_LENGTH], int * dst, int profitDst);
void makeLog(char * str);

#endif