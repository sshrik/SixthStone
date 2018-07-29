#include "sixthStone.h"

int getCandidate(char plate[][PLATE_MAX], cord2D lenCord, cord2D *candCord, int turn)	{
	// Check where can turn`th player put stone.
	int candidateWeight[PLATE_MAX][PLATE_MAX] = {{0,}};
	int i, j, candidateNum = 0;
	int maxWeight = -1;
	cord2D temp;

	for(i = 0; i < lenCord.x; i++)	{
		for(j = 0; j < lenCord.y; j++)	{
			temp.x = i;
			temp.y = j;
			if(canPut(plate, lenCord, temp, turn) == YES)	{
				candidateWeight[i][j] = getCandWeight(plate, temp, turn);
				maxWeight = (maxWeight > candidateWeight[i][j] ? maxWeight : candidateWeight[i][j]);
			}
		}
	}

	for(i = 0; i < lenCord.x; i++)	{
		for(j = 0; j < lenCord.y; j++)	{
			if(candidateWeight[i][j] == maxWeight) {
				candCord[candidateNum].x = i;
				candCord[candidateNum].y = j;
				candidateNum++;
			}
		}
	}

	return candidateNum;
}

int getCandWeight(char plate[][PLATE_MAX], cord2D temp, int turn)	{
	int weight = 0;
	int w, b, i;
	w = 0; b = 0;

	if(!isOutOfPlate(temp, 2, EAST))	{
		if(plate[temp.x][temp.y + 1] == WHITE) w++;
		else if(plate[temp.x][temp.y + 1] == BLACK) b++;
	}
	if(!isOutOfPlate(temp, 2, WEST))	{
		if(plate[temp.x][temp.y - 1] == WHITE) w++;
		else if(plate[temp.x][temp.y - 1] == BLACK) b++;
	}
	if(!isOutOfPlate(temp, 2, SOUTH))	{
		if(plate[temp.x + 1][temp.y] == WHITE) w++;
		else if(plate[temp.x + 1][temp.y] == BLACK) b++;
	}
	if(!isOutOfPlate(temp, 2, NORTH))	{
		if(plate[temp.x - 1][temp.y] == WHITE) w++;
		else if(plate[temp.x - 1][temp.y] == BLACK) b++;
	}

	if(!isOutOfPlate(temp, 2, EAST_SOUTH))	{
		if(plate[temp.x + 1][temp.y] + 1 == WHITE) w++;
		else if(plate[temp.x + 1][temp.y + 1] == BLACK) b++;
	}
	if(!isOutOfPlate(temp, 2, EAST_NORTH))	{
		if(plate[temp.x - 1][temp.y + 1] == WHITE) w++;
		else if(plate[temp.x - 1][temp.y + 1] == BLACK) b++;
	}
	if(!isOutOfPlate(temp, 2, WEST_SOUTH))	{
		if(plate[temp.x + 1][temp.y - 1] == WHITE) w++;
		else if(plate[temp.x + 1][temp.y - 1] == BLACK) b++;
	}
	if(!isOutOfPlate(temp, 2, WEST_NORTH))	{
		if(plate[temp.x - 1][temp.y - 1] == WHITE) w++;
		else if(plate[temp.x - 1][temp.y - 1] == BLACK) b++;
	}

	if(turn == WHITE)	{
		for(i = 1; i <= w; i++)	{
			weight += i;
		}
		for(i = 1; i <= b; i++)	{
			weight -= i;
		}
	}
	else if(turn == BLACK)	{
		for(i = 1; i <= w; i++)	{
			weight -= i;
		}
		for(i = 1; i <= b; i++)	{
			weight += i;
		}
	}
	return weight;
}

int getWinState(char plate[][PLATE_MAX], cord2D *cord, int turn)	{
	// Find is plate have win state. If exist, return dir. Else, return "NO";
	int i, j, k, dir = NO;
	cord2D temp;

	for(i = 0; i < PLATE_MAX; i++)	{
		for(j = 0; j < PLATE_MAX; j++)	{
			for(k = 1; k <= 8; k++)	{
				temp.x = i;
				temp.y = j;
				if(isWinState(plate, temp, turn, k) == YES)	{
					// In case of outside starting case, move opposite direction.
					switch (k) {
						case EAST:
							cord->x = i;
							if (j == 0) cord->y = j - 1;
							else cord->y = j;
							break;
						case WEST:
							cord->x = i;
							if (j == PLATE_MAX - 1)	cord->y = j + 1;
							else cord->y = j;
							break;
						case SOUTH:
							if (i == 0)	cord->x = i - 1;
							else cord->x = i;
							cord->y = j;
							break;
						case NORTH:
							if (i == PLATE_MAX - 1)	cord->x = i + 1;
							else cord->x = i;
							cord->y = j;
							break;

						case EAST_SOUTH:
							if (i == 0)	cord->x = i - 1;
							else cord->x = i;
							if (j == 0) cord->y = j - 1;
							else cord->y = j;
							break;
						case EAST_NORTH:
							if (i == 0)	cord->x = i - 1;
							else cord->x = i;
							if (j == PLATE_MAX - 1)	cord->y = j + 1;
							else cord->y = j;
							break;
						case WEST_SOUTH:
							if (i == PLATE_MAX - 1)	cord->x = i + 1;
							else cord->x = i;
							if (j == 0) cord->y = j - 1;
							else cord->y = j;
							break;
						case WEST_NORTH:
							if (i == PLATE_MAX - 1)	cord->x = i + 1;
							else cord->x = i;
							if (j == PLATE_MAX - 1)	cord->y = j + 1;
							else cord->y = j;
							break;
					}
					return k;
				}
			}
		}
	}

	return dir;
}

int isWinState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir)	{
	// Does start form plate[cord.x][cord.y] to + 8 to "dir" direction is win state?
	int i, count = 0;
	
	if ((cord.x == 0 || cord.y == 0 || cord.x == PLATE_MAX - 1 || cord.y == PLATE_MAX - 1) && plate[cord.x][cord.y] == turn) {
		// For start with cordinate (0, _), (_, 0), (18, _) or (_, 18) State check.
		if (isOutOfPlate(cord, 7, dir)) return NO;
		switch (dir) {
			case EAST:	// Y++
				if (plate[cord.x][cord.y + 6] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x][cord.y + i] == turn)	count++;
					else if(plate[cord.x][cord.y + i] != EMPTY)	{
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;
			case WEST:	// Y--
				if (plate[cord.x][cord.y - 6] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x][cord.y - i] == turn)	count++;
					else if (plate[cord.x][cord.y - i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;
			case SOUTH:	// X++
				if (plate[cord.x + 6][cord.y] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x + i][cord.y] == turn)	count++;
					else if (plate[cord.x + i][cord.y] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;
			case NORTH:	// X--
				if (plate[cord.x - 6][cord.y] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x - i][cord.y] == turn)	count++;
					else if (plate[cord.x - i][cord.y] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;

			case EAST_SOUTH:
				if (plate[cord.x + 6][cord.y + 6] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x + i][cord.y + i] == turn)	count++;
					else if (plate[cord.x + i][cord.y + i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;
			case EAST_NORTH:
				if (plate[cord.x - 6][cord.y + 6] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x - i][cord.y + i] == turn)	count++;
					else if (plate[cord.x - i][cord.y + i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;
			case WEST_SOUTH:
				if (plate[cord.x + 6][cord.y - 6] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x + i][cord.y - i] == turn)	count++;
					else if (plate[cord.x + i][cord.y - i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;
			case WEST_NORTH:
				if (plate[cord.x - 6][cord.y - 6] == turn) return NO;
				for (i = 0; i < 6; i++) {
					if (plate[cord.x - i][cord.y - i] == turn)	count++;
					else if (plate[cord.x - i][cord.y - i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count >= 4 ? YES : NO;
				break;
			default:
				break;
		}
	}
	else {
		if (isOutOfPlate(cord, 8, dir))	return NO;
		switch (dir) {
			case EAST:	// Y++
				if (plate[cord.x][cord.y] != turn && plate[cord.x][cord.y + 7] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x][cord.y + i] == turn) count++;
						else if (plate[cord.x][cord.y + i] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;
			case WEST:	// Y--
				if (plate[cord.x][cord.y] != turn && plate[cord.x][cord.y - 7] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x][cord.y - i] == turn) count++;
						else if (plate[cord.x][cord.y - i] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;
			case SOUTH:	// X++
				if (plate[cord.x][cord.y] != turn && plate[cord.x + 7][cord.y] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x + i][cord.y] == turn) count++;
						else if (plate[cord.x + i][cord.y] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;
			case NORTH:	// X--
				if (plate[cord.x][cord.y] != turn && plate[cord.x - 7][cord.y] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x - i][cord.y] == turn) count++;
						else if (plate[cord.x - i][cord.y] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;

			case EAST_SOUTH:	// X++ Y++
				if (plate[cord.x][cord.y] != turn && plate[cord.x + 7][cord.y + 7] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x + i][cord.y + i] == turn) count++;
						else if (plate[cord.x + i][cord.y + i] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;
			case EAST_NORTH:	// X-- Y++
				if (plate[cord.x][cord.y] != turn && plate[cord.x - 7][cord.y + 7] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x - i][cord.y + i] == turn) count++;
						else if (plate[cord.x - i][cord.y + i] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;
			case WEST_SOUTH:	// X++ Y--
				if (plate[cord.x][cord.y] != turn && plate[cord.x + 7][cord.y - 7] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x + i][cord.y - i] == turn) count++;
						else if (plate[cord.x + i][cord.y - i] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;
			case WEST_NORTH:	// X-- Y--
				if (plate[cord.x][cord.y] != turn && plate[cord.x - 7][cord.y - 7] != turn) {
					// if 4 or 5 turn`th stone exist, return true.
					for (i = 1; i <= 6; i++) {
						if (plate[cord.x - i][cord.y - i] == turn) count++;
						else if (plate[cord.x - i][cord.y - i] != EMPTY) {
							count = 0;
							break;
						}
					}
					return count >= 4 ? YES : NO;
				}
				break;
			default:
				break;
		}
	}
	return NO;
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

void doWin(char plate[][PLATE_MAX], cord2D temp , int dir, cord2D * next, int turn)	{
	// Start from plate[cord.x, cord.y], direction dir is winning state, so put nextX and nextY to finish game.
	int i, count = 0;

	switch (dir) {
		case EAST:	// Y++
			for (i = 1; i < 8; i++) {
				if (plate[temp.x][temp.y + i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST:	// Y--
			for (i = 1; i < 8; i++) {
				if (plate[temp.x][temp.y - i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y - i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case SOUTH:	// X++
			for (i = 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case NORTH: // X--
			for (i = 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y;
					count++;
				}
				if (count == 2) break;
			}
			break;

		case EAST_SOUTH:	// X++ y++
			for (i = 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y + i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case EAST_NORTH:	// X-- y++
			for (i = 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y + i] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST_SOUTH:	// X++ y--
			for (i = 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y - i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y - i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST_NORTH:	// x-- y--
			for (i = 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y - i] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y - i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		default:
			break;
	}
	if (next[0].x != -1)	put(plate, next[0], turn);
	if (next[1].x != -1)	put(plate, next[1], turn);
}

void doSheild(char plate[][PLATE_MAX], cord2D temp, int dir, cord2D * next, int turn) {
	// Start from plate[cord.x, cord.y], direction dir is winning state, so put nextX and nextY to finish game.
	int i, count = 0;
	// For processing ___OOOO_.
	int nonTurnIndex = 0;
	int oppo;

	oppo = turn == BLACK ? WHITE : BLACK;
	
	for (i = 0; i < 8; i++) {
		switch (dir) {
			case EAST:
				if (plate[temp.x][temp.y + i] != oppo && plate[temp.x][temp.y + i + 1] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST:
				if (plate[temp.x][temp.y - i] != oppo && plate[temp.x][temp.y - i - 1] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;
			case SOUTH:
				if (plate[temp.x + i][temp.y] != oppo && plate[temp.x + i + 1][temp.y] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;
			case NORTH:
				if (plate[temp.x - i][temp.y] != oppo && plate[temp.x - i - 1][temp.y] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;

			case EAST_SOUTH:
				if (plate[temp.x + i][temp.y + i] != oppo && plate[temp.x + i + 1][temp.y + i + 1] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;
			case EAST_NORTH:
				if (plate[temp.x - i][temp.y + i] != oppo && plate[temp.x - i - 1][temp.y + i + 1] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST_SOUTH:
				if (plate[temp.x + i][temp.y - i] != oppo && plate[temp.x + i + 1][temp.y - i - 1] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST_NORTH:
				if (plate[temp.x - i][temp.y - i] != oppo && plate[temp.x - i - 1][temp.y - i - 1] == oppo) {
					nonTurnIndex = i;
					break;
				}
				break;
			default:
				break;
		}
	}
	switch (dir) {
	case EAST:
		if (plate[temp.x][temp.y + nonTurnIndex] == EMPTY) {
			next[count].x = temp.x;
			next[count].y = temp.y + nonTurnIndex;
			count++;
		}
		break;
	case WEST:
		if (plate[temp.x][temp.y - nonTurnIndex] == EMPTY) {
			next[count].x = temp.x;
			next[count].y = temp.y - nonTurnIndex;
			count++;
		}
		break;
	case SOUTH:
		if (plate[temp.x + nonTurnIndex][temp.y] == EMPTY) {
			next[count].x = temp.x + nonTurnIndex;
			next[count].y = temp.y;
			count++;
		}
		break;
	case NORTH:
		if (plate[temp.x - nonTurnIndex][temp.y] == EMPTY) {
			next[count].x = temp.x - nonTurnIndex;
			next[count].y = temp.y;
			count++;
		}
		break;

	case EAST_SOUTH:
		if (plate[temp.x + nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
			next[count].x = temp.x + nonTurnIndex;
			next[count].y = temp.y + nonTurnIndex;
			count++;
		}
		break;
	case EAST_NORTH:
		if (plate[temp.x - nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
			next[count].x = temp.x - nonTurnIndex;
			next[count].y = temp.y + nonTurnIndex;
			count++;
		}
		break;
	case WEST_SOUTH:
		if (plate[temp.x + nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
			next[count].x = temp.x - nonTurnIndex;
			next[count].y = temp.y - nonTurnIndex;
			count++;
		}
		break;
	case WEST_NORTH:
		if (plate[temp.x - nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
			next[count].x = temp.x - nonTurnIndex;
			next[count].y = temp.y - nonTurnIndex;
			count++;
		}
		break;
	default:
		break;
	}

	switch (dir) {
		case EAST:	// Y++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x][temp.y + i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y + i;
					break;
				}
			}
			break;
		case WEST:	// Y--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x][temp.y - i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y - i;
					break;
				}
			}
			break;
		case SOUTH:	// X++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y;
					break;
				}
			}
			break;
		case NORTH: // X--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y;
					break;
				}
			}
			break;

		case EAST_SOUTH:	// X++ y++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y + i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y + i;
					break;
				}
			}
			break;
		case EAST_NORTH:	// X-- y++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y + i] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y + i;
					break;
				}
			}
			break;
		case WEST_SOUTH:	// X++ y--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y - i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y - i;
					break;
				}
			}
			break;
		case WEST_NORTH:	// x-- y--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y - i] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y - i;
					break;
				}
			}
			break;
		default:
			break;
	}
	if (next[0].x != -1)	put(plate, next[0], turn);
	if (next[1].x != -1)	put(plate, next[1], turn);
}

void sixthStoneBot(char plate[][PLATE_MAX], cord2D lenCord, cord2D *next, cord2D *before, int turn)	{
	// Main AI. Return at next[2].
	// Make candidate proper number ( 50 );
	cord2D oppoCandCord[PLATE_MAX * PLATE_MAX], myCandCord[PLATE_MAX * PLATE_MAX];
	cord2D temp;
	char tempPlate[PLATE_MAX][PLATE_MAX];
	int oppoCandNum = 0;
	int myCandNum = 0;
	int highestWeight = 0;
	int i, oppo, oppoWeight, myWeight, index[2], loseDir, count = 0;

	// turn mean my color, oppo mean other`s color.
	if(turn == BLACK) oppo = WHITE;
	else oppo = BLACK;

	next[0].x = -1;	next[0].y = -1;
	next[1].x = -1;	next[1].y = -1;
	index[0] = 0;	index[1] = 0;

	//Search in case we can win.
	if((loseDir = getWinState(plate, &temp, turn)) != NO) {
		printf("Winning State find at (%d, %d) with dir %d.\n", temp.x, temp.y, loseDir);
		//system("pause");
		doWin(plate, temp, loseDir, next, turn);
	}

	// Check oppo`s win state. ( 4 ~ 5 continuos state. )
	else if((loseDir = getWinState(plate, &temp, oppo)) != NO)	{
		printf("Losing State find at (%d, %d) with dir %d.\n", temp.x, temp.y, loseDir);
		//system("pause");
		doSheild(plate, temp, loseDir, next, turn);
	}

	if(next[1].x == -1 && next[0].x != -1) {
		// Calculate opposite turn`s highest plate.
		oppoCandNum = getCandidate(plate, lenCord, oppoCandCord, oppo);
		oppoWeight = getCandWeight(plate, oppoCandCord[oppoCandNum - 1], oppo);
	
		for(i = 0; i < oppoCandNum; i++)	{
			memcpy(tempPlate, plate, sizeof(char) * PLATE_MAX * PLATE_MAX);
			tempPlate[oppoCandCord[i].x][oppoCandCord[i].y] = turn;
			myCandNum = getCandidate(tempPlate, lenCord, myCandCord, turn);
			myWeight = getCandWeight(tempPlate, oppoCandCord[oppoCandNum - 1], turn);	
			if(myWeight > highestWeight) index[1] = i;
		}	
		next[1].x = oppoCandCord[index[1]].x;
		next[1].y = oppoCandCord[index[1]].y;
		put(plate, next[1], turn);
	}
	else if(next[1].x == -1 && next[0].x == -1 ){	
		// Calculate opposite turn`s highest plate.
		oppoCandNum = getCandidate(plate, lenCord, oppoCandCord, oppo);
		oppoWeight = getCandWeight(plate, oppoCandCord[oppoCandNum - 1], oppo);

		for(i = 0; i < oppoCandNum; i++)	{
			memcpy(tempPlate, plate, sizeof(char) * PLATE_MAX * PLATE_MAX);
			tempPlate[oppoCandCord[i].x][oppoCandCord[i].y] = turn;
			myCandNum = getCandidate(tempPlate, lenCord, myCandCord, turn);
			myWeight = getCandWeight(tempPlate, oppoCandCord[oppoCandNum - 1], turn);	
			if(myWeight > highestWeight) index[0] = i;
		}

		next[0].x = oppoCandCord[index[0]].x;
		next[0].y = oppoCandCord[index[0]].y;
	
		put(plate, next[0], turn);
		highestWeight = 0;
	
		// Calculate opposite turn`s highest plate.
		oppoCandNum = getCandidate(plate, lenCord, oppoCandCord, oppo);
		oppoWeight = getCandWeight(plate, oppoCandCord[oppoCandNum - 1], oppo);
	
		for(i = 0; i < oppoCandNum; i++)	{
			memcpy(tempPlate, plate, sizeof(char) * PLATE_MAX * PLATE_MAX);
			tempPlate[oppoCandCord[i].x][oppoCandCord[i].y] = turn;
			myCandNum = getCandidate(tempPlate, lenCord, myCandCord, turn);
			myWeight = getCandWeight(tempPlate, oppoCandCord[oppoCandNum - 1], turn);	
			if(myWeight > highestWeight) index[1] = i;
		}	
		next[1].x = oppoCandCord[index[1]].x;
		next[1].y = oppoCandCord[index[1]].y;
		put(plate, next[1], turn);
	}
}
