#include "sixthStone.h"

void display(char plate[][PLATE_MAX])	{
	// Clear monitor and Display plate with x - y number.
	// int i, j;

	int candidateWeight[PLATE_MAX][PLATE_MAX] = { { 0, } };
	int i, j, candidateNum = 0;
	int maxWeight = -1;
	int turn;
	cord2D temp;
	cord2D candCord[PLATE_MAX * PLATE_MAX];

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

	turn = WHITE;

	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp.x = i;
			temp.y = j;
			if (canPut(plate, temp, turn) == YES) {
				candidateWeight[i][j] = getCandWeight(plate, temp, turn);
				maxWeight = (maxWeight > candidateWeight[i][j] ? maxWeight : candidateWeight[i][j]);
			}
		}
	}

	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			if (candidateWeight[i][j] == maxWeight) {
				candCord[candidateNum].x = i;
				candCord[candidateNum].y = j;
				candidateNum++;
			}
		}
	}
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

int getStateDir(char* state, char plate[][PLATE_MAX], int stateLen, int dir, int stateValue)	{
	char * str = (char *)malloc(sizeof(char) * stateLen);
	int i, j, addAll = 0;

	for(i = 0; i < PLATE_MAX; i++)	{
		for(j = 0; j < PLATE_MAX; j++)	{
			if(getStrDir(str, plate, i, j, stateLen, dir) > 0)	{
				if(strncmp(str, state, stateLen) == 0)	{
					addAll += stateValue;
				}
			}
			memset(str, 0x00, stateLen);
		}
	}
	free(str);
	return addAll;
}

int getStrDir(char* str, char plate[][PLATE_MAX], int x, int y, int stateLen, int dir)	{
	int i, j;
	switch(dir)	{
		case SOUTH:	// X ++
			if(x + stateLen - 1 >= PLATE_MAX) return -1;
			else {
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x + i][y];
				}
				return 1;
			}
			break;
		case NORTH:	// X --
			if(x - stateLen + 1 < 0 ) return -1;
			else {
				// Copy in reverse order... [W][W][C][B][E] ... -> this change to "EBCWW".
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x - i][y];
				}
				return 1;
			}
			break;
		case EAST:	// Y ++
			if(y + stateLen - 1 >= PLATE_MAX) return -1;
			else {
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x][y + i];
				}
				return 1;
			}
			break;
		case WEST:	// Y --
			if(y - stateLen + 1 < 0 ) return -1;
			else {
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x][y - i];
				}
				return 1;
			}
			break;

		case EAST_SOUTH:	// X ++ Y ++
			if(x + stateLen - 1 >= PLATE_MAX || y + stateLen - 1 >= PLATE_MAX) return -1;
			else {
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x + i][y + i];
				}
				return 1;
			}
			break;
		case WEST_SOUTH:	// X ++ Y --
			if(x + stateLen - 1 >= PLATE_MAX || y - stateLen + 1 < 0) return -1;
			else {
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x + i][y - i];
				}
				return 1;
			}
			break;
		case EAST_NORTH:	// X -- Y ++
			if(x - stateLen + 1 < 0 || y + stateLen - 1 >= PLATE_MAX) return -1;
			else {
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x - i][y + i];
				}
				return 1;
			}
			break;
		case WEST_NORTH:	// X -- Y --
			if(x - stateLen + 1 < 0 || y + stateLen - 1 < 0) return -1;
			else {
				for(i = 0; i < stateLen; i++)	{
					str[i] = plate[x - i][y - i];
				}
				return 1;
			}
			break;
		default:
			break;
	}
	return -1;
}

void initPlate(char plate[][PLATE_MAX], int blockNum)	{
	int i, j, randNum;
	int blockCount = 0;
	int randomBlockSize = rand() % 6 * 2;

	// Make block( "C" ).
	for(i = 0; i < PLATE_MAX; i++)	{
		for (j = 0; j < PLATE_MAX; j++) {
			plate[i][j] = EMPTY;
		}
	}

	//in real match, the random block stones number should be even number.
	//So we have to check if the randomBlcok(x,y) is duplicated, but let's pass it!!.
	for (int k = 0; k < randomBlockSize; k++) {
		plate[rand() % PLATE_MAX][rand() % PLATE_MAX] = BLOCK;
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
		return cord.x + next - 1 >= PLATE_MAX && cord.y + next - 1 >= PLATE_MAX;
		break;
	case EAST_NORTH:	// X-- Y++
		return cord.x - next + 1 < 0 && cord.y + next - 1 >= PLATE_MAX;
		break;
	case WEST_SOUTH:	// X++ Y--
		return cord.x + next - 1 >= PLATE_MAX && cord.y - next + 1 < 0;
		break;
	case WEST_NORTH:	// X-- Y-- 
		return cord.x - next + 1 < 0 && cord.y - next + 1 < 0;
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