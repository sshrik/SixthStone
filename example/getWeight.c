#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define PLATE_MAX 19
#define STATE_MAX 10

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

void initPlate(char plate[][PLATE_MAX]); 
int getStateDir(char* state, char plate[][PLATE_MAX], int stateLen, int dir, int stateValue);
int getStrDir(char* str, char plate[][PLATE_MAX], int x, int y, int stateLen, int dir);
void display(char plate[][PLATE_MAX], int lenX, int lenY);

int main()	{
	char plate[PLATE_MAX][PLATE_MAX];
	char state[20];
	int i, var = 0;
	time_t startT, endT;

	initPlate(plate);
	
	display(plate, PLATE_MAX, PLATE_MAX);

	printf("Insert state : " );
	scanf(" %s", state);
	printf("State %s for %d length...\n", state, (int)strlen(state));
	
	startT = clock();
	for(i = 1; i < 9; i++)	{	// 1 ~ 8 is number of direction.
		var = getStateDir(state, plate, strlen(state), i, 1);
		printf(" %d var is result for dir %d.\n", var, i);
	}
	endT = clock();

	printf(" %f / %ld sec was flooded while calculating.\n", (float)(endT - startT), (CLOCKS_PER_SEC));



	return 0;
}

void initPlate(char plate[][PLATE_MAX])	{
	int i, j, r;

	srand(time(NULL));

	for(i = 0; i < PLATE_MAX; i++)	{
		for(j = 0; j < PLATE_MAX; j++)	{
			r = rand() % 4;
			switch(r)	{
				case 0:
					plate[i][j] = WHITE;
					break;
				case 1:
					plate[i][j] = BLACK;
					break;
				case 2:
					plate[i][j] = BLOCK;
					break;
				case 3:
					plate[i][j] = EMPTY;
					break;
				default:
					break;
			}
		}
	}
}

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
