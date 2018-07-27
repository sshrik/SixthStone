#include "sixthStone.h"

int getCandidate(char plate[][PLATE_MAX], int * candX, int * candY, int turn);
int getWinState(char plate[][PLATE_MAX], int * X, int * Y, int turn);
int isWinState(char plate[][PLATE_MAX], int x, int y, int turn, int dir);
int isOutOfPlate(int x, int y, int next, int dir);
int getState(char plate[][PLATE_MAX], int x, int y, int turn, int dir);
void doWin(char plate[][PLATE_MAX], int * X, int * Y, int dir, int * nextX, int * nextY, int turn);
void getNext(char plate[][PLATE_MAX], int *candX, int * candY, int * nextX, int * nextY, int turn);







int getCandidate(char plate[][PLATE_MAX], int * candX, int * candY, int turn)	{
	// Check all turn`s around EMPTY.

}

int getWinState(char plate[][PLATE_MAX], int * X, int * Y, int turn)	{
	// Find is plate have win state. If exist, return dir. Else, return "NO";
	int i, j, k, dir = NO;
	for(i = 0; i < PLATE_MAX; i++)	{
		for(j = 0; j < PLATE_MAX; j++)	{
			for(k = 1; k < 8; k++)	{
				if(isWinState(plate, i, j, turn, k))	{
					*X = i;
					*Y = j;
					return k;
				}
			}
		}
	}
	return dir;
}

int isWinState(char plate[][PLATE_MAX], int x, int y, int turn, int dir)	{
	// Does [x][y] to 8 is win state?
	int i;
	if(isOutOfPlate(x, y, 8, dir)) {
		// Does state is out of state, -1 is not count stating point.
		return NO;
	}
	switch(dir)	{
		case EAST:	// Y++
			if(plate[x][y] == turn && plate[x][y + 7] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
			}
			break;
		case WEST:	// Y--
			break;
		case SOUTH:	// X++
			break;
		case NORTH:	// X--
			break;

		case EAST_SOUTH:	// X++ Y++
			break;
		case EAST_NORTH:	// X-- Y++
			break;
		case WEST_SOUTH:	// X++ Y--
			break;
		case WEST_NORTH:	// X-- Y--
			break;
		
		default:
			break;
	}
}

int isOutOfPlate(int x, int y, int next, int dir)	{
	// Check x + next or y - next are out of state.
	switch(dir)	{
		case EAST:	// Y++
			return y + next - 1 < PLATE_MAX;
			break;
		case WEST:	// Y--
			return y - next + 1>= 0;
			break;
		case SOUTH:	// X++
			return x + next - 1< PLATE_MAX;
			break;
		case NORTH:	// X--
			return x - next + 1 >= 0;
			break;

		case EAST_SOUTH:	// X++ Y++
			return x + next  - 1 < PLATE_MAX && y + next - 1 < PLATE_MAX;
			break;
		case EAST_NORTH:	// X-- Y++
			return x - next + 1 >= 0 && y + next - 1 < PLATE_MAX;
			break;
		case WEST_SOUTH:	// X++ Y--
			return x + next - 1 < PLATE_MAX && y - next + 1 >= 0;
			break;
		case WEST_NORTH:	// X-- Y-- 
			return x - next + 1 >= 0 && y - next + 1 >= 0;
			break;
		
		default:
			break;
	}
	
}

int getState(char plate[][PLATE_MAX], int x, int y, int turn, int dir)	{
	int val = -1;
	// Stone can be WHITE. BLACK, EMPTY, BLOCK <=> BLACK, WHITE
	// if XOOO___X == 2 score.	cause 3 continuos and 1 blocked.
	// if X__OO__X == 2 score.	cause 2 continuos and no blocked.
	// if XOO_____ == 1 score.	cause 2 continuos and 1 blocked.
	// if XOOX____ == 0 score.	cause 2 continuos but all blocked.
	// if XOOXOXOX == 0 score.	cause this have no-meaning.
	if(isOutOfPlate(x, y, 8, dir)) {
		// Does state is out of state, -1 is not count stating point.
		return NO;
	}

	switch(dir)	{
		case EAST:	// Y++
			break;
		case WEST:	// Y--
			break;
		case SOUTH:	// X++
			break;
		case NORTH:	// X--
			break;

		case EAST_SOUTH:	// X++ Y++
			break;
		case EAST_NORTH:	// X-- Y++
			break;
		case WEST_SOUTH:	// X++ Y--
			break;
		case WEST_NORTH:	// X-- Y--
			break;
		
		default:
			break;
	}
}

void doWin(char plate[][PLATE_MAX], int * X, int * Y, int dir, int * nextX, int * nextY, int turn)	{
	// Start from x, y, direction dir is winning state, so do nextX and nextY to finish game.
	switch(dir)	{
		case EAST:	// Y++
			break;
		case WEST:	// Y--
			break;
		case SOUTH:	// X++
			break;
		case NORTH:	// X--
			break;

		case EAST_SOUTH:	// X++ Y++
			break;
		case EAST_NORTH:	// X-- Y++
			break;
		case WEST_SOUTH:	// X++ Y--
			break;
		case WEST_NORTH:	// X-- Y--
			break;
		
		default:
			break;
	}
}

void getNext(char plate[][PLATE_MAX], int *candX, int * candY, int * nextX, int * nextY, int turn);

