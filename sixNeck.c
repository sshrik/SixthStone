/*
 *	@Title : Youk-mok main file.
 *	@Writer : CHJ
 *	@Date : 2018 07 17 ~
 */

#include<stdio.h>
#include<stdlib.h>

#define BLACK 1
#define WHITE 2
#define EMPTY 0
#define NO -1
#define YES -2

#define LEN_MAX 20

int whoWin(int plate[][LEN_MAX], int lenX, int lenY, int X, int Y);
void put(int plate[][LEN_MAX], int X, int Y, int what);
int canPut(int plate[][LEN_MAX], int lenX, int lenY, int X, int Y, int what);
void changeTurn(int * turn);
void display(int plate[][LEN_MAX], int lenX, int lenY);

int main()	{
	int lenX, lenY, x, y;
	int plate[LEN_MAX][LEN_MAX]= {{0,},};
	int turn = BLACK;

	x = 0;	lenX = LEN_MAX;
	y = 0;	lenY = LEN_MAX;

		
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
}

int whoWin(int plate[][LEN_MAX], int lenX, int lenY, int X, int Y)	{
	// East
	int what = plate[X][Y];
	int i;

	for(i = 0; i < 6; i++)	{
		if(Y + i >= lenY) break;
		else if(plate[X][Y + i] != what)	break;
	}
	if(i == 6) return plate[X][Y];

	// West
	for(i = 0; i < 6; i++)	{
		if(Y - i < 0) break;
		else if(plate[X][Y - i] != what)	break;
	}
	if(i == 6) return plate[X][Y];

	// South
	for(i = 0; i < 6; i++)	{
		if(X + i >= lenX) break;
		else if(plate[X + i][Y] != what)	break;
	}
	if(i == 6) return plate[X][Y];
	
	// North
	for(i = 0; i < 6; i++)	{
		if(X - i < 0) break;
		else if(plate[X - i][Y] != what)	break;
	}
	if(i == 6) return plate[X][Y];
	
	// East-South
	for(i = 0; i < 6; i++)	{
		if(Y + i >= lenY || X + i >= lenX) break;
		else if(plate[X + i][Y + i] != what)	break;
	}
	if(i == 6) return plate[X][Y];
	
	// East-North
	for(i = 0; i < 6; i++)	{
		if(Y + i >= lenY || X - i < 0) break;
		else if(plate[X - i][Y + i] != what)	break;
	}
	if(i == 6) return plate[X][Y];
	
	// West-South
	for(i = 0; i < 6; i++)	{
		if(Y - i < 0 || X + i >= lenX) break;
		else if(plate[X + i][Y - i] != what)	break;
	}
	if(i == 6) return plate[X][Y];

	// West-Nort
	for(i = 0; i < 6; i++)	{
		if(Y - i < 0 || X - i < 0) break;
		else if(plate[X - i][Y - i] != what)	break;
	}
	if(i == 6) return plate[X][Y];
	
	return NO;
}

void put(int plate[][LEN_MAX], int X, int Y, int what)	{
	plate[X][Y] = what;
}

int canPut(int plate[][LEN_MAX], int lenX, int lenY, int X, int Y, int what)	{
	if(X >= lenX || Y >= lenY)	{
		// If x or y coordinate is over len max,
		return NO;
	}	
	else if(plate[X][Y] != EMPTY)	{
		// If plate[x][y] is filled with other stone,
		return NO;
	}
	return YES;
}

void changeTurn(int * turn)	{
	if(*turn == BLACK) *turn = WHITE;
	else *turn = BLACK;	
}

void display(int plate[][LEN_MAX], int lenX, int lenY)	{
	// Clear monitor and Display plate with x - y number.
	int i, j;
	
	system("clear");

	// Printf with color.
	// 30 default	31 Bright Red	32 Green	33 Yellow	34 Blue	35 Violet	36 Bright Blue	37 Bright White

	printf("%c[1;%dm",27, 37);
	printf("*\t");
	printf("%c[0m",27);

	for(j = 0; j < lenY; j++) {
		printf("%c[1;%dm", 27, 37);
		printf("%d\t", j);
		printf("%c[0m",27); 
	}
	printf("\n");

	for(i = 0; i < lenX; i++)	{
		printf("%c[1;%dm",27, 37);
		printf("%d\t", i);
		printf("%c[0m",27); 
		for(j = 0; j < lenY; j++)	{
			switch(plate[i][j])	{
				case BLACK:
					printf("%c[1;%dm", 27, 34);
					printf("B\t");
					printf("%c[0m",27); 
					break;
				case WHITE:
					printf("%c[1;%dm", 27, 37);
					printf("W\t");
					printf("%c[0m",27);
					break;
				case EMPTY:
					printf("%c[1;%dm", 27, 30);
					printf("E\t");
					printf("%c[0m",27);
					break;
				default:
					break;
			}
		}
		printf("\n");
	}
}
