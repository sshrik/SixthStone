#include "sixthStone.h"

void display(char plate[][PLATE_MAX])	{
	// Clear monitor and Display plate with x - y number.
	// int i, j;

	int candidateWeight[PLATE_MAX][PLATE_MAX] = { { 0, } };
	int i, j, candidateNum = 0;
	int maxWeight = -1;
	int turn, tPlate[PLATE_MAX][PLATE_MAX];
	int weightListB[10] = { 1, 3, 6, 700, 900, 1, 3, 6, 10, 15 };
	int weightListW[10] = { 1, 3, 6, 200, 300, 0, 0, 0, 0, 0 };
	cord2D temp;
	cord2D candCord[PLATE_MAX * PLATE_MAX];
	cord2D before[2];

	memset(before, 0x10, sizeof(cord2D) * 2);

	system("cls");

	// Printf with color.
	// 30 default	31 Bright Red	32 Green	33 Yellow	34 Blue	35 Violet	36 Bright Blue	37 Bright White

	printf("%c[1;%dm",27, 37);
	printf("*\t");
	printf("%c[0m",27);

	for(j = 0; j < PLATE_MAX; j++) {
		printf("%c[1;%dm", 27, 37);
		printf("%d\t", j);
		printf("%c[0m",27); 
	}
	printf("\n");

	for (i = 0; i < PLATE_MAX; i++) {
		printf("%c[1;%dm", 27, 37);
		printf("%d\t", i);
		printf("%c[0m", 27);
		for (j = 0; j < PLATE_MAX; j++) {
			switch (plate[i][j]) {
			case BLACK:
				printf("%c[1;%dm", 27, 34);
				printf("B\t");
				printf("%c[0m", 27);
				break;
			case WHITE:
				printf("%c[1;%dm", 27, 37);
				printf("W\t");
				printf("%c[0m", 27);
				break;
			case EMPTY:
				printf("%c[1;%dm", 27, 30);
				printf("E\t");
				printf("%c[0m", 27);
				break;
			case BLOCK:
				printf("%c[1;%dm", 27, 36);
				printf("C\t");
				printf("%c[0m", 27);
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
	/*
	turn = WHITE;

	changeBlocking(plate, tPlate, turn);
	candidateNum = getCandidate(tPlate, candidateWeight, candCord, before, 20, weightListW, turn);
	printf("%c[1;%dm", 27, 37);
	printf("*\t");
	printf("%c[0m", 27);

	for (j = 0; j < PLATE_MAX; j++) {
		printf("%c[1;%dm", 27, 37);
		printf("%d\t", j);
		printf("%c[0m", 27);
	}
	printf("\n");

	for (i = 0; i < PLATE_MAX; i++) {
		printf("%c[1;%dm", 27, 37);
		printf("%d\t", i);
		printf("%c[0m", 27); 
		for (j = 0; j < PLATE_MAX; j++) {
			switch (plate[i][j]) {
			case BLACK:
				printf("%c[1;%dm", 27, 34);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			case WHITE:
				printf("%c[1;%dm", 27, 37);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			case EMPTY:
				printf("%c[1;%dm", 27, 30);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			case BLOCK:
				printf("%c[1;%dm", 27, 36);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			default:
				break;
			}
		}
		printf("\n");
	}

	turn = BLACK;

	changeBlocking(plate, tPlate, turn);
	candidateNum = getCandidate(tPlate, candidateWeight, candCord, before, 20, weightListB, turn);
	printf("%c[1;%dm", 27, 37);
	printf("*\t");
	printf("%c[0m", 27);

	for (j = 0; j < PLATE_MAX; j++) {
		printf("%c[1;%dm", 27, 37);
		printf("%d\t", j);
		printf("%c[0m", 27);
	}
	printf("\n");

	for (i = 0; i < PLATE_MAX; i++) {
		printf("%c[1;%dm", 27, 37);
		printf("%d\t", i);
		printf("%c[0m", 27);
		for (j = 0; j < PLATE_MAX; j++) {
			switch (plate[i][j]) {
			case BLACK:
				printf("%c[1;%dm", 27, 34);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			case WHITE:
				printf("%c[1;%dm", 27, 37);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			case EMPTY:
				printf("%c[1;%dm", 27, 30);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			case BLOCK:
				printf("%c[1;%dm", 27, 36);
				printf("%d\t", candidateWeight[i][j]);
				printf("%c[0m", 27);
				break;
			default:
				break;
			}
		}
		printf("\n");
	}*/
}

int allWhoWin(char plate[][PLATE_MAX]) {
	int i, j;
	cord2D temp[2];
	char cPlate[PLATE_MAX][PLATE_MAX];

	changeBlocking(plate, cPlate, BLACK);

	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp[0].x = i;	temp[1].x = i;
			temp[0].y = j;	temp[1].y = j;
			if (whoWin(cPlate, temp, BLACK) != NO) {
				return cPlate[i][j];
			}
		}
	}

	changeBlocking(plate, cPlate, WHITE);

	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp[0].x = i;	temp[1].x = i;
			temp[0].y = j;	temp[1].y = j;
			if (whoWin(cPlate, temp, WHITE) != NO) {
				return cPlate[i][j];
			}
		}
	}

	return NO;
}

int whoWin(char plate[][PLATE_MAX], cord2D* cord, int turn)	{
	// TODO : Need to add 7-stones to loose.
	char what = turn;
	int c, i;
	char cPlate[PLATE_MAX][PLATE_MAX];

	changeBlocking(plate, cPlate, turn);

	for(c = 0; c < 2; c++)	{
		// East
		if(!isOutOfPlate(cord[c], 6, EAST)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x][cord[c].y + i] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// West
		if(!isOutOfPlate(cord[c], 6, WEST)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x][cord[c].y - i] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}
		
		// South
		if(!isOutOfPlate(cord[c], 6, SOUTH)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x + i][cord[c].y] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// North
		if(!isOutOfPlate(cord[c], 6, NORTH)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x - i][cord[c].y] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// East-South
		if(!isOutOfPlate(cord[c], 6, EAST_SOUTH)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x + i][cord[c].y + i] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// East-North
		if(!isOutOfPlate(cord[c], 6, EAST_NORTH)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x - i][cord[c].y + i] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// West-South
		if(!isOutOfPlate(cord[c], 6, WEST_SOUTH)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x + i][cord[c].y - i] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}
		// West-Nort
		if(!isOutOfPlate(cord[c], 6, WEST_NORTH)){
			for(i = 0; i < 6; i++)	{
				if(cPlate[cord[c].x - i][cord[c].y - i] != what)	break;
			}
			if(i == 6) return cPlate[cord[c].x][cord[c].y];
		}
	}
	return NO;
}

void put(char plate[][PLATE_MAX], cord2D cord, int what)	{
	plate[cord.x][cord.y] = what;
}

int canPut(char plate[][PLATE_MAX], cord2D cord, int what)	{
	if(isOutOfPlate(cord, 0, 1))	return NO;
	else if(plate[cord.x][cord.y] != EMPTY)	{
		// If plate[x][y] is filled with other stone,
		return NO;
	}
	return YES;
}

void changeTurn(int * turn)	{
	if(*turn == BLACK) *turn = WHITE;
	else *turn = BLACK;	
}

void initPlate(char plate[][PLATE_MAX], int blockNum)	{
	int i, j, randNum;
	int blockCount = 0;
	int randomBlockSize = rand() % 6 * 2;
	cord2D temp;

	// Make block( "C" ).
	for(i = 0; i < PLATE_MAX; i++)	{
		for (j = 0; j < PLATE_MAX; j++) {
			plate[i][j] = EMPTY;
		}
	}

	for (int k = 0; k < randomBlockSize; k++) {
		do {
			temp.x = rand() % PLATE_MAX;
			temp.y = rand() % PLATE_MAX;
		} while (plate[temp.x][temp.y] != EMPTY);
		plate[temp.x][temp.y] = BLOCK;
	}
}

int isOutOfPlate(cord2D cord, int next, int dir) {
	// Check x + next or y - next are out of plate.
	switch (dir) {
	case EAST:	// Y++
		return cord.y + next - 1 >= PLATE_MAX;
		break;
	case WEST:	// Y--
		return cord.y - next + 1 < 0;
		break;
	case SOUTH:	// X++
		return cord.x + next - 1 >= PLATE_MAX;
		break;
	case NORTH:	// X--
		return cord.x - next + 1 < 0;
		break;

	case EAST_SOUTH:	// X++ Y++
		return cord.x + next - 1 >= PLATE_MAX || cord.y + next - 1 >= PLATE_MAX;
		break;
	case EAST_NORTH:	// X-- Y++
		return cord.x - next + 1 < 0 || cord.y + next - 1 >= PLATE_MAX;
		break;
	case WEST_SOUTH:	// X++ Y--
		return cord.x + next - 1 >= PLATE_MAX || cord.y - next + 1 < 0;
		break;
	case WEST_NORTH:	// X-- Y-- 
		return cord.x - next + 1 < 0 || cord.y - next + 1 < 0;
		break;
	default:
		break;
	}
}

void changeBlocking(char plate[][PLATE_MAX], char changePlate[][PLATE_MAX], int turn) {
	int i, j;
	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			changePlate[i][j] = plate[i][j] == BLOCK ? turn : plate[i][j];
		}
	}
}

int isSevenStone(char plate[][PLATE_MAX], cord2D cord, int turn) {
	// If do at cord location with turn`th turn, does occured seven stone?
	int i, j, k, dir;
	char start = plate[cord.x][cord.y];
	char tempPlate[PLATE_MAX][PLATE_MAX];

	cord2D temp;

	changeBlocking(plate, tempPlate, turn);
	tempPlate[cord.x][cord.y] = turn;

	for (j = 0; j < PLATE_MAX; j++) {
		for (k = 0; k < PLATE_MAX; k++) {
			temp.x = j;
			temp.y = k;
			for(dir = 1; dir <= 8; dir++)	{
				isOutOfPlate(temp, 7, dir);
				switch (dir) {
					case EAST:
						for (i = 0; i < 7; i++) {
							if (plate[j][k + i] != start || plate[j][k + i] != BLOCK) {
								return NO;
							}
						}
						break;
					case WEST:
						for (i = 0; i < 7; i++) {
							if (plate[j][k - i] != start || plate[j][k - i] != BLOCK) {
								return NO;
							}
						}
						break;
					case SOUTH:
						for (i = 0; i < 7; i++) {
							if (plate[j + i][k] != start || plate[j + i][k] != BLOCK) {
								return NO;
							}
						}
						break;
					case NORTH:
						for (i = 0; i < 7; i++) {
							if (plate[j - i][k] != start || plate[j - i][k] != BLOCK) {
								return NO;
							}
						}
						break;


					case EAST_SOUTH:
						for (i = 0; i < 7; i++) {
							if (plate[j + i][k + i] != start || plate[j + i][k + i] != BLOCK) {
								return NO;
							}
						}
						break;
					case EAST_NORTH:
						for (i = 0; i < 7; i++) {
							if (plate[j - i][k + i] != start || plate[j - i][k + i] != BLOCK) {
								return NO;
							}
						}
						break;
					case WEST_SOUTH:
						for (i = 0; i < 7; i++) {
							if (plate[j + i][k - i] != start || plate[j + i][k - i] != BLOCK) {
								return NO;
							}
						}
						break;
					case WEST_NORTH:
						for (i = 0; i < 7; i++) {
							if (plate[j - i][k - i] != start || plate[j - i][k - i] != BLOCK) {
								return NO;
							}
						}
						break;
				}
			}
		}
	}

	if (isOutOfPlate(cord, 7, dir)) return NO;

	

	return YES;
}