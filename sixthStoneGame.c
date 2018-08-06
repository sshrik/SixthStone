#include "sixthStone.h"

int consoleMain()	{
	char plate[PLATE_MAX][PLATE_MAX];
	int turn = WHITE;
	int i, blockNum;
	cord2D nextPut[2], before[2];
	int weightListB[10] = { 1, 3, 6, 200, 300, 1, 3, 6, 10, 15 };
	int weightListW[10] = { 1, 3, 6, 200, 300, 0, 0, 0, 0, 0 };

	srand(time(NULL));
	nextPut[0].x = -1;
	nextPut[0].y = -1;
	nextPut[1].x = -1;
	nextPut[1].y = -1;

	blockNum = 5;

	initPlate(plate, blockNum);
	sixthStoneBot(plate, nextPut, before, 1, weightListB, turn);

	do {
		changeTurn(&turn);
		display(plate);
		if(turn == WHITE)	{
			sixthStoneBot(plate, nextPut, before, 2, weightListW, turn);
		}
		else {
			sixthStoneBot(plate, nextPut, before, 2, weightListB,turn);
			/*
			for(i = 0; i < 2; i++)	{
				scanf(" %d %d", &nextPut[i].x, &nextPut[i].y);
				while(canPut(plate, nextPut[i], turn) == NO)	{
					printf("%c[1;%dm", 27, 31);
					printf("You can`t put there.\n");
					printf("%c[0m",27);
					scanf(" %d %d", &nextPut[i].x, &nextPut[i].y);
				}
				put(plate, nextPut[i], turn);
				display(plate, lenCord);
			}
			*/
		}
	}	while(whoWin(plate, nextPut, turn) == NO);
	display(plate);
	if(turn == WHITE) printf("White Win!\n");
	else printf("Black Win!\n");
	system("pause");

	return 0;
}
