#include "sixStone.h"

void display(char plate[][PLATE_MAX], int lenX, int lenY)	{
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
				case BLOCK:
					printf("%c[1;%dm", 27, 36);
					printf("C\t");
					printf("%c[0m",27);
					break;
				default:
					break;
			}
		}
		printf("\n");
	}
}

int whoWin(char plate[][PLATE_MAX], int lenX, int lenY, int X, int Y)	{
	// TODO : Need to add 7-stones to loose.
	// TODO : Need to check with isOutOfPlate() function to check outside. 
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

void put(char plate[][PLATE_MAX], int X, int Y, int what)	{
	plate[X][Y] = what;
}

int canPut(char plate[][PLATE_MAX], int lenX, int lenY, int X, int Y, int what)	{
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
