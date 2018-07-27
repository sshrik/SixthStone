#include "sixthStone.h"



int getCandidate(char plate[][PLATE_MAX], cord2D *candCord, int turn)	{
	// Check where can turn`th player put stone.

}

int getWinState(char plate[][PLATE_MAX], cord2D *cord, int turn)	{
	// Find is plate have win state. If exist, return dir. Else, return "NO";
	int i, j, k, dir = NO;
	cord2D temp;

	for(i = 0; i < PLATE_MAX; i++)	{
		for(j = 0; j < PLATE_MAX; j++)	{
			for(k = 1; k < 8; k++)	{
				temp.x = i;
				temp.y = j;
				if(isWinState(plate, temp, turn, k))	{
					cord->x = i;
					cord->y = j;
					return k;
				}
			}
		}
	}
	return dir;
}

int isWinState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir)	{
	// Does start form plate[cord.x][cord.y] to + 8 to "dir" direction is win state?
	int i;
	
	if(isOutOfPlate(cord, 8, dir)) {
		return NO;
	}

	switch(dir)	{
		case EAST:	// Y++
			if(plate[cord.x][cord.y] == turn && plate[cord.x][cord.y + 7] != turn) {
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

int isOutOfPlate(cord2D cord, int next, int dir)	{
	// Check x + next or y - next are out of plate.
	switch(dir)	{
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
			return cord.x + next  - 1 >= PLATE_MAX && cord.y + next - 1 >= PLATE_MAX;
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

int getState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir)	{
	int val = -1;
	/* Stone can be WHITE. BLACK, EMPTY, BLOCK <=> BLACK, WHITE
	* if XOOO___X == 2 score.	cause 3 continuos and 1 blocked.
	* if X__OO__X == 2 score.	cause 2 continuos and no blocked.
	* if XOO_____ == 1 score.	cause 2 continuos and 1 blocked.
	* if XOOX____ == 0 score.	cause 2 continuos but all blocked.
	* if XOOXOXOX == 0 score.	cause this have no-meaning. 
	*/
	if(isOutOfPlate(cord, 8, dir)) {
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

void doWin(char plate[][PLATE_MAX], cord2D * cord , int dir, cord2D * next, int turn)	{
	// Start from plate[cord.x, cord.y], direction dir is winning state, so put nextX and nextY to finish game.
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

void getNext(char plate[][PLATE_MAX], cord2D *candCord, cord2D *next, int turn)	{
	// Put next 2 stone with given candidate Cordinate, candCord at "turn"`th turn.

}

void sixthStoneBot(char plate[][PLATE_MAX], cord2D lenCord, cord2D *next, cord2D *before, int turn)	{
	// Main AI. Return at next[2].
	srand(time(NULL));
	while(canPut(plate, lenCord, next[0], turn) == NO)	{
		next[0].x = rand() % PLATE_MAX;
		next[0].y = rand() % PLATE_MAX;
	}
	put(plate, next[0], turn);
	while(canPut(plate, lenCord, next[1], turn) == NO)	{
		next[1].x = rand() % PLATE_MAX;
		next[1].y = rand() % PLATE_MAX;
	}
	put(plate, next[1], turn);
}
