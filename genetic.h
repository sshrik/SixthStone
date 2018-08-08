#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define GEN_LENGTH 10
#define GEN_MAX 300
#define PARENT_MAX 40
#define CHILD_NUM 8
#define GENERATION_MAX PARENT_MAX * PARENT_MAX * CHILD_NUM

#define MUT_PER 70

void initGen(int gen[][GEN_LENGTH]);
void makeChild(int par[][GEN_LENGTH], int child[][GEN_LENGTH]);
int calcProfit(int * child, int * dst);
void makeMutate(int * gen);
void choosePar(int gen[][GEN_LENGTH], int par[][GEN_LENGTH], int * dst);
int findProfit(int child[][GEN_LENGTH], int * dst, int profitDst);