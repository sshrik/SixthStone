#include "sixthStone.h"

int main()	{
	int lenX, lenY, x, y;
	char plate[PLATE_MAX][PLATE_MAX];
	int turn = BLACK;

	x = 0;	lenX = PLATE_MAX;
	y = 0;	lenY = PLATE_MAX;

	initPlate(plate);
		
	do {
		display(plate, lenX, lenY);
		scanf(" %d %d", &x, &y);
		while(canPut(plate, lenX, lenY, x, y, turn) == NO)	{

			printf("%c[1;%dm", 27, 31);
			printf("You can`t put there.\n");
			printf("%c[0m",27);
			scanf(" %d %d", &x, &y);
		}
		put(plate, x, y, turn);
		changeTurn(&turn);
	}	while(whoWin(plate, lenX, lenY, x, y) == NO);
	if(turn == WHITE) printf("White Win!\n");
	else printf("Black Win!\n");
	return 0;

	return 0;
}
