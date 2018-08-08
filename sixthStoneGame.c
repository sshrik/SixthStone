/*
#include "sixthStone.h"

int main()	{
	char plate[PLATE_MAX][PLATE_MAX];
	int turn = BLACK;
	int i, blockNum;
	cord2D nextPut[2], before[2];
	int weightListB[20] = { 1, 3, 6, 700, 900, 1, 3, 6, 10, 15, 1, 3, 6, 700, 900, 1, 3, 6, 10, 15 };
	int weightListW[20] = { 1, 3, 6, 200, 300, 0, 0, 0, 0, 0, 1, 3, 6, 200, 300, 0, 0, 0, 0, 0 };
	int nowTurn = 1;

	srand(time(NULL));
	memset(nextPut, -1, sizeof(cord2D) * 2);
	memset(before, 0x09, sizeof(cord2D) * 2);

	blockNum = 5;

	initPlate(plate, blockNum);
	sixthStoneBot(plate, nextPut, before, 1, weightListB, turn, nowTurn++);

	do {
		changeTurn(&turn);
		display(plate);
//		printf("Put at %d, %d and %d, %d\n", nextPut[0].x, nextPut[0].y, nextPut[1].x, nextPut[1].y);
//		system("pause");

		if(turn == WHITE)	{
			sixthStoneBot(plate, nextPut, before, 2, weightListW, turn, nowTurn++);
		}
		else {
			sixthStoneBot(plate, nextPut, before, 2, weightListB, turn, nowTurn++);
			
		}
		memcpy(before, nextPut, sizeof(cord2D) * 2);
		if (allWhoWin(plate) == EMPTY) {
			break;
		}
	}	while(allWhoWin(plate) == NO);
	display(plate);
	if(turn == WHITE) printf("White Win!\n");
	else printf("Black Win!\n");
	system("pause");

	return 0;
}
*/

#include "genetic.h"
#include "sixthStone.h"

int main() {
	int par[PARENT_MAX][GEN_LENGTH], child[GENERATION_MAX][GEN_LENGTH];
	char log[5000];
	int i, j, logB, profitIndex, count = 0;

	srand(time(NULL));
	initGen(child);

	while (1) {
		// Init for log.
		memset(log, 0x00, sizeof(char) * 5000);
		logB = 0;

		logB = sprintf(log + logB, "%d th generation now proceeding...\n", count);
		makeLog(log);

		printf("%d th generation now proceeding...\n", count++);
		choosePar(child, par);
		makeChild(par, child);

		for (i = 0; i < GENERATION_MAX; i++) {
			makeMutate(child[i]);
		}

		printf("profit parent : [ ");
		for (i = 0; i < GEN_LENGTH; i++) {
			printf("%2d  ", par[0][i]);
		}
		printf("]\n");
	}

	printf("profit child : [ ");
	for (i = 0; i < GEN_LENGTH; i++) {
		printf("%2d  ", child[profitIndex][i]);
	}
	printf("]\n");
	system("pause");

	return 0;
}