#include "sixthStone.h"

int getCandidate(char plate[][PLATE_MAX], int candidateWeight[][PLATE_MAX], cord2D *candCord, cord2D * before, int candLimit, int * weightList, int turn)	{
	// Check where can turn`th player put stone.
	int i, j, k, dir;
	int candNum = candLimit;
	cord2D temp;
	int* candWeightList = (int *)malloc(sizeof(int) * (candLimit * 2));
	int* candLengthList = (int *)malloc(sizeof(int) * (candLimit * 2));
	int realWeightList[10];
	int count = getWinningStateNumber(plate, 3, turn);

	// Init array.
	memset(candidateWeight, 0, sizeof(int) * PLATE_MAX * PLATE_MAX);
	memset(candCord, -1, sizeof(cord2D) * candLimit);
	memset(candWeightList, 0x00, sizeof(int) * candLimit);
	memset(candLengthList, 0x00, sizeof(int) * candLimit);

	if (count > WINSTATE_MAX) {
		memcpy(realWeightList, &weightList[10], sizeof(int) * 10);
	}
	else {
		memcpy(realWeightList, &weightList[0], sizeof(int) * 10);
	}

	// Add weight for winning state.
	for (i = 0; i < PLATE_MAX; i++) {
		for(j = 0; j < PLATE_MAX; j++)	{
			temp.x = i;
			temp.y = j;
			for (dir = 1; dir <= 8; dir++) {
				if (turn == WHITE) {
					if (isWinState(plate, temp, WHITE, dir, 5) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[4]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 4) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[3]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 3) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[2]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 2) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[1]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 1) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[0]);
					}

					if (isWinState(plate, temp, BLACK, dir, 5) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[9]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 4) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[8]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 3) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[7]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 2) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[6]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 1) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[5]);
					}
				}
				else {
					if (isWinState(plate, temp, BLACK, dir, 5) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[4]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 4) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[3]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 3) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[2]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 2) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[1]);
					}
					else if (isWinState(plate, temp, BLACK, dir, 1) == YES) {
						addWeight(plate, temp, candidateWeight, dir, realWeightList[0]);
					}

					if (isWinState(plate, temp, WHITE, dir, 5) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[9]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 4) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[8]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 3) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[7]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 2) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[6]);
					}
					else if (isWinState(plate, temp, WHITE, dir, 1) == YES) {
						addWeight(plate, temp, candidateWeight, dir, -1 * realWeightList[5]);
					}
				}
			}
		}
	}

	// Add weight for around state.
	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp.x = i;
			temp.y = j;
			if (canPut(plate, temp, turn) == YES) {
				candidateWeight[i][j] += getCandWeight(plate, temp, turn);
			}
		}
	}

	// Calc candidate and sort with given limit number / candidate weight.
	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			if (plate[i][j] != EMPTY) continue;
			temp.x = i;
			temp.y = j;
			for (k = 0; k < candLimit - 1; k++) {
				if (getLength(temp, before[0]) + getLength(temp, before[1]) > 49 * 4) {
					// 너무 멀리 있는 후보 빼고 조사.
					continue;
				}
				else if (candCord[k].x == -1) {
					// If candCord have another room...
					memcpy(&candCord[k], &temp, sizeof(cord2D));
					candWeightList[k] = candidateWeight[i][j];
					candLengthList[k] = getLength(temp, before[0]) + getLength(temp, before[1]);
					break;
				}
				else if(candWeightList[k] < candidateWeight[i][j]){
					memmove(&candWeightList[k + 1], &candWeightList[k], sizeof(int) * (candLimit - k - 1));
					memmove(&candCord[k + 1], &candCord[k], sizeof(cord2D) * (candLimit - k - 1));
					memmove(&candLengthList[k + 1], &candLengthList[k], sizeof(int) * (candLimit - k - 1));

					memcpy(&candCord[k], &temp, sizeof(cord2D));
					candWeightList[k] = candidateWeight[i][j];
					candLengthList[k] = getLength(temp, before[0]) + getLength(temp, before[1]);
					break;
				}
				else {
					if (candWeightList[k] >= candidateWeight[i][j] && candWeightList[k + 1] < candidateWeight[i][j] && candCord[k + 1].x != -1) {
						memmove(&candWeightList[k + 2], &candWeightList[k + 1], sizeof(int) * (candLimit - k - 2));
						memmove(&candCord[k + 2], &candCord[k + 1], sizeof(cord2D) * (candLimit - k - 2));
						memmove(&candLengthList[k + 2], &candLengthList[k + 1], sizeof(int) * (candLimit - k - 2));

						memcpy(&candCord[k + 1], &temp, sizeof(cord2D));
						candWeightList[k + 1] = candidateWeight[i][j];
						candLengthList[k + 1] = getLength(temp, before[0]) + getLength(temp, before[1]);
						break;
					}
					else if (candWeightList[k] >= candidateWeight[i][j] && candWeightList[k + 1] == candidateWeight[i][j] && candCord[k + 1].x != -1) {
						if (candLengthList[k] >= getLength(temp, before[0]) + getLength(temp, before[1])) {
							memmove(&candWeightList[k + 2], &candWeightList[k + 1], sizeof(int) * (candLimit - k - 2));
							memmove(&candCord[k + 2], &candCord[k + 1], sizeof(cord2D) * (candLimit - k - 2));
							memmove(&candLengthList[k + 2], &candLengthList[k + 1], sizeof(int) * (candLimit - k - 2));

							memcpy(&candCord[k + 1], &temp, sizeof(cord2D));
							candWeightList[k + 1] = candidateWeight[i][j];
							candLengthList[k + 1] = getLength(temp, before[0]) + getLength(temp, before[1]);
							break;
						}
					}
				}
			}
		}
	}

	// Cut - off non-useless candidate.
	for (i = 0; i < candLimit; i++) {
		if (isSevenStone(plate, candCord[i], turn) == YES) {
			memmove(&candCord[i], &candCord[i + 1], sizeof(cord2D) * (candLimit - i - 1));
			memmove(&candWeightList[i], &candWeightList[i + 1], sizeof(int) * (candLimit - i));
			memmove(&candLengthList[i], &candLengthList[i + 1], sizeof(int) * (candLimit - i));
			i--;
			candNum--;
		}
	}
	
	for (i = 1; i < candNum; i++) {
		if (candWeightList[i] * 100 < candWeightList[0] * 80) {
			memmove(&candCord[i], &candCord[i + 1], sizeof(cord2D) * (candLimit - i - 1));
			memmove(&candWeightList[i], &candWeightList[i + 1], sizeof(int) * (candLimit - i));
			memmove(&candLengthList[i], &candLengthList[i + 1], sizeof(int) * (candLimit - i));
			i--;
			candNum--;
		}
	}


	free(candLengthList);
	free(candWeightList);
	
	return candNum;
}

int getCandWeight(char plate[][PLATE_MAX], cord2D temp, int turn)	{
	int weight = 0;
	int w, b, i, dir;
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

void addWeight(char plate[][PLATE_MAX], cord2D temp, int cordWeight[][PLATE_MAX], int dir, int addNum) {
	int i;

	if (plate[temp.x][temp.y] == EMPTY) {
		cordWeight[temp.x][temp.y] -= addNum;
	}
	switch (dir) {
	case EAST:
		for (i = 1; i < 7; i++)	if (plate[temp.x][temp.y + i] == EMPTY) cordWeight[temp.x][temp.y + i] += addNum;
		// In case i == 7;
		if (plate[temp.x][temp.y + i] == EMPTY) cordWeight[temp.x][temp.y + i] -= addNum;
		break;
	case WEST:
		for (i = 1; i < 7; i++)	if (plate[temp.x][temp.y - i] == EMPTY) cordWeight[temp.x][temp.y - i] += addNum;
		// In case i == 7;
		if (plate[temp.x][temp.y - i] == EMPTY) cordWeight[temp.x][temp.y - i] -= addNum;
		break;
	case SOUTH:
		for (i = 1; i < 7; i++)	if (plate[temp.x + i][temp.y] == EMPTY) cordWeight[temp.x + i][temp.y] += addNum;
		// In case i == 7;
		if (plate[temp.x + i][temp.y] == EMPTY) cordWeight[temp.x + i][temp.y] -= addNum;
		break;
	case NORTH:
		for (i = 1; i < 7; i++)	if (plate[temp.x - i][temp.y] == EMPTY) cordWeight[temp.x - i][temp.y] += addNum;
		// In case i == 7;
		if (plate[temp.x - i][temp.y] == EMPTY) cordWeight[temp.x - i][temp.y] -= addNum;
		break;

	case EAST_SOUTH:
		for (i = 1; i < 7; i++)	if (plate[temp.x + i][temp.y + i] == EMPTY) cordWeight[temp.x + i][temp.y + i] += addNum;
		// In case i == 7;
		if (plate[temp.x + i][temp.y + i] == EMPTY) cordWeight[temp.x + i][temp.y + i] -= addNum;
		break;
	case EAST_NORTH:
		for (i = 1; i < 7; i++)	if (plate[temp.x - i][temp.y + i] == EMPTY) cordWeight[temp.x - i][temp.y + i] += addNum;
		// In case i == 7;
		if (plate[temp.x - i][temp.y + i] == EMPTY) cordWeight[temp.x - i][temp.y + i] -= addNum;
		break;
	case WEST_SOUTH:
		for (i = 1; i < 7; i++)	if (plate[temp.x + i][temp.y - i] == EMPTY) cordWeight[temp.x + i][temp.y - i] += addNum;
		// In case i == 7;
		if (plate[temp.x + i][temp.y - i] == EMPTY) cordWeight[temp.x + i][temp.y - i] -= addNum;
		break;
	case WEST_NORTH:
		for (i = 1; i < 7; i++)	if (plate[temp.x - i][temp.y - i] == EMPTY) cordWeight[temp.x - i][temp.y - i] += addNum;
		// In case i == 7;
		if (plate[temp.x - i][temp.y - i] == EMPTY) cordWeight[temp.x - i][temp.y - i] -= addNum;
		break;
	}
}

void changeCandWeight(char plate[][PLATE_MAX], int candidateWeight[][PLATE_MAX], cord2D before, int * weightList, int turn) {
	// Change special cordination ( before ) around weight.
	int dir, i;
	for (dir = 1; dir <= 8; dir++) {
		for (i = -7; i < 8; i++) {

		}
	}
}

int getLength(cord2D a, cord2D b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

long long int calcWeight(int cordWeight[][PLATE_MAX]) {
	long long int result = 0;
	int i, j;
	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			result += cordWeight[i][j];
		}
	}
	return result;
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
				if(isWinState(plate, temp, turn, k, 4) == YES || isWinState(plate, temp, turn, k, 5) == YES)	{
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

int isWinState(char plate[][PLATE_MAX], cord2D cord, int turn, int dir, int continum)	{
	// Does start form plate[cord.x][cord.y] to + 8 to "dir" direction is win state?
	int i, count = 0;
	
	switch (dir) {
	case EAST:	// Y++
		if (cord.y == PLATE_MAX - 7) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x][cord.y + i] == turn) count++;
					else if (plate[cord.x][cord.y + i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x][cord.y + 7] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x][cord.y + i] == turn) count++;
				else if (plate[cord.x][cord.y + i] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;
	case WEST:	// Y--
		if (cord.y == 6) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x][cord.y - i] == turn) count++;
					else if (plate[cord.x][cord.y - i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x][cord.y - 7] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x][cord.y - i] == turn) count++;
				else if (plate[cord.x][cord.y - i] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;
	case SOUTH:	// X++
		if (cord.x == PLATE_MAX - 7) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x + i][cord.y] == turn) count++;
					else if (plate[cord.x + i][cord.y] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x + 7][cord.y] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x + i][cord.y] == turn) count++;
				else if (plate[cord.x + i][cord.y] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;
	case NORTH:	// X--
		if (cord.x == 6) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x - i][cord.y] == turn) count++;
					else if (plate[cord.x - i][cord.y] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x - 7][cord.y] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x - i][cord.y] == turn) count++;
				else if (plate[cord.x - i][cord.y] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;

	case EAST_SOUTH:	// X++ Y++
		if (cord.y == PLATE_MAX - 7 || cord.x == PLATE_MAX - 7) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x + i][cord.y + i] == turn) count++;
					else if (plate[cord.x + i][cord.y + i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x + 7][cord.y + 7] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x + i][cord.y + i] == turn) count++;
				else if (plate[cord.x + i][cord.y + i] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;
	case EAST_NORTH:	// X-- Y++
		if (cord.y == PLATE_MAX - 7 || cord.x == 6) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x - i][cord.y + i] == turn) count++;
					else if (plate[cord.x - i][cord.y + i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x - 7][cord.y + 7] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x - i][cord.y + i] == turn) count++;
				else if (plate[cord.x - i][cord.y + i] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;
	case WEST_SOUTH:	// X++ Y--
		if (cord.y == 6 || cord.x == PLATE_MAX - 7) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x + i][cord.y - i] == turn) count++;
					else if (plate[cord.x + i][cord.y - i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x + 7][cord.y - 7] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x + i][cord.y - i] == turn) count++;
				else if (plate[cord.x + i][cord.y - i] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;
	case WEST_NORTH:	// X-- Y--
		if (cord.y == 6 || cord.x == 6) {
			if (plate[cord.x][cord.y] != turn) {
				// if 4 or 5 turn`th stone exist, return true.
				for (i = 1; i <= 6; i++) {
					if (plate[cord.x - i][cord.y - i] == turn) count++;
					else if (plate[cord.x - i][cord.y - i] != EMPTY) {
						count = 0;
						break;
					}
				}
				return count == continum ? YES : NO;
			}
		}
		if (isOutOfPlate(cord, 8, dir))	return NO;
		if (plate[cord.x][cord.y] != turn && plate[cord.x - 7][cord.y - 7] != turn) {
			// if 4 or 5 turn`th stone exist, return true.
			for (i = 1; i <= 6; i++) {
				if (plate[cord.x - i][cord.y - i] == turn) count++;
				else if (plate[cord.x - i][cord.y - i] != EMPTY) {
					count = 0;
					break;
				}
			}
			return count == continum ? YES : NO;
		}
		break;
	default:
		break;
	}
	return NO;
}

void doWin(char plate[][PLATE_MAX], cord2D temp , int dir, cord2D * next, int turn)	{
	// Start from plate[cord.x, cord.y], direction dir is winning state, so put nextX and nextY to finish game.
	int i, count = 0;

	if (temp.x == -1 || temp.y == -1 || temp.x == PLATE_MAX || temp.y == PLATE_MAX) {
		if (temp.x == -1) {
			temp.x += 1;
		}
		if (temp.y == -1) {
			temp.y += 1;
		}
		if (temp.x == PLATE_MAX) {
			temp.x -= 1;
		}
		if (temp.y == PLATE_MAX) {
			temp.y -= 1;
		}
		switch (dir) {
		case EAST:	// Y++
			for (i = 0; i < 7; i++) {
				if (plate[temp.x][temp.y + i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST:	// Y--
			for (i = 0; i < 7; i++) {
				if (plate[temp.x][temp.y - i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y - i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case SOUTH:	// X++
			for (i = 0; i < 7; i++) {
				if (plate[temp.x + i][temp.y] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case NORTH: // X--
			for (i = 0; i < 7; i++) {
				if (plate[temp.x - i][temp.y] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y;
					count++;
				}
				if (count == 2) break;
			}
			break;

		case EAST_SOUTH:	// X++ y++
			for (i = 0; i < 7; i++) {
				if (plate[temp.x + i][temp.y + i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case EAST_NORTH:	// X-- y++
			for (i = 0; i < 7; i++) {
				if (plate[temp.x - i][temp.y + i] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST_SOUTH:	// X++ y--
			for (i = 0; i < 7; i++) {
				if (plate[temp.x + i][temp.y - i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y - i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST_NORTH:	// x-- y--
			for (i = 0; i < 7; i++) {
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
	}
	else {
		switch (dir) {
		case EAST:	// Y++
			for (i = 1; i < 7; i++) {
				if (plate[temp.x][temp.y + i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST:	// Y--
			for (i = 1; i < 7; i++) {
				if (plate[temp.x][temp.y - i] == EMPTY) {
					next[count].x = temp.x;
					next[count].y = temp.y - i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case SOUTH:	// X++
			for (i = 1; i < 7; i++) {
				if (plate[temp.x + i][temp.y] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case NORTH: // X--
			for (i = 1; i < 7; i++) {
				if (plate[temp.x - i][temp.y] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y;
					count++;
				}
				if (count == 2) break;
			}
			break;

		case EAST_SOUTH:	// X++ y++
			for (i = 1; i < 7; i++) {
				if (plate[temp.x + i][temp.y + i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case EAST_NORTH:	// X-- y++
			for (i = 1; i < 7; i++) {
				if (plate[temp.x - i][temp.y + i] == EMPTY) {
					next[count].x = temp.x - i;
					next[count].y = temp.y + i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST_SOUTH:	// X++ y--
			for (i = 1; i < 7; i++) {
				if (plate[temp.x + i][temp.y - i] == EMPTY) {
					next[count].x = temp.x + i;
					next[count].y = temp.y - i;
					count++;
				}
				if (count == 2) break;
			}
			break;
		case WEST_NORTH:	// x-- y--
			for (i = 1; i < 7; i++) {
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
	}
	if (next[0].x != -1) {
		if (isSevenStone(plate, next[1], turn) == NO) {
			put(plate, next[0], turn);
		}
		else {
			//
		}
	}
	if (next[1].x != -1) {
		if (isSevenStone(plate, next[1], turn) == NO) {
			put(plate, next[1], turn);
		}
		else {
			//
		}
	}
}

void doSheild(char plate[][PLATE_MAX], cord2D temp, int dir, cord2D * next, int turn) {
	// Start from plate[cord.x, cord.y], direction dir is winning state, so put nextX and nextY to finish game.
	int i, count = 0;
	// For processing ___OOOO_.
	int nonTurnIndex = -1;
	int oppo, flag = 0;

	oppo = turn == BLACK ? WHITE : BLACK;

	if (temp.x == -1 || temp.y == -1 || temp.x == PLATE_MAX || temp.y == PLATE_MAX) {
		// For start from -1 state.
		if (temp.x == -1) {
			temp.x += 1;
		}
		if (temp.y == -1) {
			temp.y += 1;
		}
		if (temp.x == PLATE_MAX) {
			temp.x -= 1;
		}
		if (temp.y == PLATE_MAX) {
			temp.y -= 1;
		}

		for (i = 0; i < 7; i++) {
			switch (dir) {
			case EAST:
				if (plate[temp.x][temp.y + i] != oppo && (plate[temp.x][temp.y + i + 1] == oppo || plate[temp.x][temp.y + i + 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST:
				if (plate[temp.x][temp.y - i] != oppo && (plate[temp.x][temp.y - i - 1] == oppo || plate[temp.x][temp.y - i - 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case SOUTH:
				if (plate[temp.x + i][temp.y] != oppo && (plate[temp.x + i + 1][temp.y] == oppo || plate[temp.x + i + 1][temp.y] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case NORTH:
				if (plate[temp.x - i][temp.y] != oppo && (plate[temp.x - i - 1][temp.y] == oppo || plate[temp.x - i - 1][temp.y] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;

			case EAST_SOUTH:
				if (plate[temp.x + i][temp.y + i] != oppo && (plate[temp.x + i + 1][temp.y + i + 1] == oppo || plate[temp.x + i + 1][temp.y + i + 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case EAST_NORTH:
				if (plate[temp.x - i][temp.y + i] != oppo && (plate[temp.x - i - 1][temp.y + i + 1] == oppo || plate[temp.x - i - 1][temp.y + i + 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST_SOUTH:
				if (plate[temp.x + i][temp.y - i] != oppo && (plate[temp.x + i + 1][temp.y - i - 1] == oppo || plate[temp.x + i + 1][temp.y - i - 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST_NORTH:
				if (plate[temp.x - i][temp.y - i] != oppo && (plate[temp.x - i - 1][temp.y - i - 1] == oppo || plate[temp.x - i - 1][temp.y - i - 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			default:
				break;
			}
			if (nonTurnIndex != -1) break;
		}

		switch (dir) {
		case EAST:
			if (plate[temp.x][temp.y + nonTurnIndex] == EMPTY) {
				next->x = temp.x;
				next->y = temp.y + nonTurnIndex;
				count++;
			}
			break;
		case WEST:
			if (plate[temp.x][temp.y - nonTurnIndex] == EMPTY) {
				next->x = temp.x;
				next->y = temp.y - nonTurnIndex;
				count++;
			}
			break;
		case SOUTH:
			if (plate[temp.x + nonTurnIndex][temp.y] == EMPTY) {
				next->x = temp.x + nonTurnIndex;
				next->y = temp.y;
				count++;
			}
			break;
		case NORTH:
			if (plate[temp.x - nonTurnIndex][temp.y] == EMPTY) {
				next->x = temp.x - nonTurnIndex;
				next->y = temp.y;
				count++;
			}
			break;

		case EAST_SOUTH:
			if (plate[temp.x + nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
				next->x = temp.x + nonTurnIndex;
				next->y = temp.y + nonTurnIndex;
				count++;
			}
			break;
		case EAST_NORTH:
			if (plate[temp.x - nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
				next->x = temp.x - nonTurnIndex;
				next->y = temp.y + nonTurnIndex;
				count++;
			}
			break;
		case WEST_SOUTH:
			if (plate[temp.x + nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
				next->x = temp.x - nonTurnIndex;
				next->y = temp.y - nonTurnIndex;
				count++;
			}
			break;
		case WEST_NORTH:
			if (plate[temp.x - nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
				next->x = temp.x - nonTurnIndex;
				next->y = temp.y - nonTurnIndex;
				count++;
			}
			break;
		default:
			break;
		}

		if (count == 1) {
			put(plate, *next, turn);
			return;
		}

		switch (dir) {
		case EAST:	// Y++
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x][temp.y + i] == EMPTY) {
					next->x = temp.x;
					next->y = temp.y + i;
					break;
				}
			}
			break;
		case WEST:	// Y--
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x][temp.y - i] == EMPTY) {
					next->x = temp.x;
					next->y = temp.y - i;
					break;
				}
			}
			break;
		case SOUTH:	// X++
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x + i][temp.y] == EMPTY) {
					next->x = temp.x + i;
					next->y = temp.y;
					break;
				}
			}
			break;
		case NORTH: // X--
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x - i][temp.y] == EMPTY) {
					next->x = temp.x - i;
					next->y = temp.y;
					break;
				}
			}
			break;

		case EAST_SOUTH:	// X++ y++
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x + i][temp.y + i] == EMPTY) {
					next->x = temp.x + i;
					next->y = temp.y + i;
					break;
				}
			}
			break;
		case EAST_NORTH:	// X-- y++
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x - i][temp.y + i] == EMPTY) {
					next->x = temp.x - i;
					next->y = temp.y + i;
					break;
				}
			}
			break;
		case WEST_SOUTH:	// X++ y--
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x + i][temp.y - i] == EMPTY) {
					next->x = temp.x + i;
					next->y = temp.y - i;
					break;
				}
			}
			break;
		case WEST_NORTH:	// x-- y--
			for (i = nonTurnIndex; i < 7; i++) {
				if (plate[temp.x - i][temp.y - i] == EMPTY) {
					next->x = temp.x - i;
					next->y = temp.y - i;
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	else {
		for (i = 1; i < 8; i++) {
			switch (dir) {
			case EAST:
				if (plate[temp.x][temp.y + i] != oppo && (plate[temp.x][temp.y + i + 1] == oppo || plate[temp.x][temp.y + i + 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST:
				if (plate[temp.x][temp.y - i] != oppo && (plate[temp.x][temp.y - i - 1] == oppo || plate[temp.x][temp.y - i - 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case SOUTH:
				if (plate[temp.x + i][temp.y] != oppo && (plate[temp.x + i + 1][temp.y] == oppo || plate[temp.x + i + 1][temp.y] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case NORTH:
				if (plate[temp.x - i][temp.y] != oppo && (plate[temp.x - i - 1][temp.y] == oppo || plate[temp.x - i - 1][temp.y] == BLOCK)) {
					nonTurnIndex = i;	
					break;
				}
				break;

			case EAST_SOUTH:
				if (plate[temp.x + i][temp.y + i] != oppo && (plate[temp.x + i + 1][temp.y + i + 1] == oppo || plate[temp.x + i + 1][temp.y + i + 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case EAST_NORTH:
				if (plate[temp.x - i][temp.y + i] != oppo && (plate[temp.x - i - 1][temp.y + i + 1] == oppo || plate[temp.x - i - 1][temp.y + i + 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST_SOUTH:
				if (plate[temp.x + i][temp.y - i] != oppo && (plate[temp.x + i + 1][temp.y - i - 1] == oppo || plate[temp.x + i + 1][temp.y - i - 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			case WEST_NORTH:
				if (plate[temp.x - i][temp.y - i] != oppo && (plate[temp.x - i - 1][temp.y - i - 1] == oppo || plate[temp.x - i - 1][temp.y - i - 1] == BLOCK)) {
					nonTurnIndex = i;
					break;
				}
				break;
			default:
				break;
			}
			if (nonTurnIndex != -1) break;
		}

		if (nonTurnIndex == -1) {
			nonTurnIndex = 0;
		}
		
		// For case to sheild X_O_OOO_.
		if (isSpecialCase(plate, temp, dir, turn)) {
			nonTurnIndex += 1;
		}

		switch (dir) {
		case EAST:
			if (plate[temp.x][temp.y + nonTurnIndex] == EMPTY) {
				next->x = temp.x;
				next->y = temp.y + nonTurnIndex;
				count++;
			}
			break;
		case WEST:
			if (plate[temp.x][temp.y - nonTurnIndex] == EMPTY) {
				next->x = temp.x;
				next->y = temp.y - nonTurnIndex;
				count++;
			}
			break;
		case SOUTH:
			if (plate[temp.x + nonTurnIndex][temp.y] == EMPTY) {
				next->x = temp.x + nonTurnIndex;
				next->y = temp.y;
				count++;
			}
			break;
		case NORTH:
			if (plate[temp.x - nonTurnIndex][temp.y] == EMPTY) {
				next->x = temp.x - nonTurnIndex;
				next->y = temp.y;
				count++;
			}
			break;

		case EAST_SOUTH:
			if (plate[temp.x + nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
				next->x = temp.x + nonTurnIndex;
				next->y = temp.y + nonTurnIndex;
				count++;
			}
			break;
		case EAST_NORTH:
			if (plate[temp.x - nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
				next->x = temp.x - nonTurnIndex;
				next->y = temp.y + nonTurnIndex;
				count++;
			}
			break;
		case WEST_SOUTH:
			if (plate[temp.x + nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
				next->x = temp.x + nonTurnIndex;
				next->y = temp.y - nonTurnIndex;
				count++;
			}
			break;
		case WEST_NORTH:
			if (plate[temp.x - nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
				next->x = temp.x - nonTurnIndex;
				next->y = temp.y - nonTurnIndex;
				count++;
			}
			break;
		default:
			break;
		}
		
		if (count == 1) {
			put(plate, * next, turn);
			return;
		}

		switch (dir) {
		case EAST:	// Y++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x][temp.y + i] == EMPTY) {
					next->x = temp.x;
					next->y = temp.y + i;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x][temp.y + nonTurnIndex] == EMPTY) {
					next->x = temp.x;
					next->y = temp.y + nonTurnIndex;
					break;
				}
			}
			break;
		case WEST:	// Y--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x][temp.y - i] == EMPTY) {
					next->x = temp.x;
					next->y = temp.y - i;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x][temp.y - nonTurnIndex] == EMPTY) {
					next->x = temp.x;
					next->y = temp.y - nonTurnIndex;
					break;
				}
			}
			break;
		case SOUTH:	// X++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y] == EMPTY) {
					next->x = temp.x + i;
					next->y = temp.y;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x + nonTurnIndex][temp.y] == EMPTY) {
					next->x = temp.x + nonTurnIndex;
					next->y = temp.y;
					break;
				}
			}
			break;
		case NORTH: // X--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y] == EMPTY) {
					next->x = temp.x - i;
					next->y = temp.y;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x - nonTurnIndex][temp.y] == EMPTY) {
					next->x = temp.x - nonTurnIndex;
					next->y = temp.y;
					break;
				}
			}
			break;

		case EAST_SOUTH:	// X++ y++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y + i] == EMPTY) {
					next->x = temp.x + i;
					next->y = temp.y + i;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x + nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
					next->x = temp.x + nonTurnIndex;
					next->y = temp.y + nonTurnIndex;
					break;
				}
			}
			break;
		case EAST_NORTH:	// X-- y++
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y + i] == EMPTY) {
					next->x = temp.x - i;
					next->y = temp.y + i;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x - nonTurnIndex][temp.y + nonTurnIndex] == EMPTY) {
					next->x = temp.x - nonTurnIndex;
					next->y = temp.y + nonTurnIndex;
					break;
				}
			}
			break;
		case WEST_SOUTH:	// X++ y--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x + i][temp.y - i] == EMPTY) {
					next->x = temp.x + i;
					next->y = temp.y - i;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x + nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
					next->x = temp.x + nonTurnIndex;
					next->y = temp.y - nonTurnIndex;
					break;
				}
			}
			break;
		case WEST_NORTH:	// x-- y--
			for (i = nonTurnIndex + 1; i < 8; i++) {
				if (plate[temp.x - i][temp.y - i] == EMPTY) {
					next->x = temp.x - i;
					next->y = temp.y - i;
					break;
				}
			}
			nonTurnIndex -= 1;
			if (next->x == -1) {
				if (plate[temp.x - nonTurnIndex][temp.y - nonTurnIndex] == EMPTY) {
					next->x = temp.x - nonTurnIndex;
					next->y = temp.y - nonTurnIndex;
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	if (next->x != -1)	put(plate,* next, turn);
}

int isSpecialCase(char plate[][PLATE_MAX], cord2D temp, int dir, int turn) {
	// Check if start with __ or X_ and end with O_ or OX.
	int oppo = turn == BLACK ? WHITE : BLACK;

	switch (dir) {
		case EAST:	// Y++
			return (plate[temp.x][temp.y + 1] == EMPTY && (plate[temp.x][temp.y + 6] == BLOCK || plate[temp.x][temp.y + 6] == oppo)) || (plate[temp.x][temp.y + 6] == EMPTY && (plate[temp.x][temp.y + 1] == BLOCK || plate[temp.x][temp.y + 1] == oppo));
			break;
		case WEST:	// Y--
			return (plate[temp.x][temp.y - 1] == EMPTY && (plate[temp.x][temp.y - 6] == BLOCK || plate[temp.x][temp.y - 6] == oppo)) || (plate[temp.x][temp.y - 6] == EMPTY && (plate[temp.x][temp.y - 1] == BLOCK || plate[temp.x][temp.y - 1] == oppo));
			break;
		case SOUTH:	// X++
			return (plate[temp.x + 1][temp.y] == EMPTY && (plate[temp.x + 6][temp.y] == BLOCK || plate[temp.x + 6][temp.y] == oppo)) || (plate[temp.x + 6][temp.y] == EMPTY && (plate[temp.x + 1][temp.y] == BLOCK || plate[temp.x + 1][temp.y] == oppo));
			break;
		case NORTH:	// X--
			return (plate[temp.x - 1][temp.y] == EMPTY && (plate[temp.x - 6][temp.y] == BLOCK || plate[temp.x - 6][temp.y] == oppo)) || (plate[temp.x - 6][temp.y] == EMPTY && (plate[temp.x - 1][temp.y] == BLOCK || plate[temp.x - 1][temp.y] == oppo));
			break;

		case EAST_SOUTH:
			return (plate[temp.x + 1][temp.y + 1] == EMPTY && (plate[temp.x + 6][temp.y + 6] == BLOCK || plate[temp.x + 6][temp.y + 6] == oppo)) || (plate[temp.x + 6][temp.y + 6] == EMPTY && (plate[temp.x + 1][temp.y + 1] == BLOCK || plate[temp.x + 1][temp.y + 1] == oppo));
			break;
		case EAST_NORTH:
			return (plate[temp.x - 1][temp.y + 1] == EMPTY && (plate[temp.x - 6][temp.y + 6] == BLOCK || plate[temp.x - 6][temp.y + 6] == oppo)) || (plate[temp.x - 6][temp.y + 6] == EMPTY && (plate[temp.x - 1][temp.y + 1] == BLOCK || plate[temp.x - 1][temp.y + 1] == oppo));
			break;
		case WEST_SOUTH:
			return (plate[temp.x + 1][temp.y - 1] == EMPTY && (plate[temp.x + 6][temp.y - 6] == BLOCK || plate[temp.x + 6][temp.y - 6] == oppo)) || (plate[temp.x + 6][temp.y - 6] == EMPTY && (plate[temp.x + 1][temp.y - 1] == BLOCK || plate[temp.x + 1][temp.y - 1] == oppo));
			break;
		case WEST_NORTH:
			return (plate[temp.x - 1][temp.y - 1] == EMPTY && (plate[temp.x - 6][temp.y - 6] == BLOCK || plate[temp.x - 6][temp.y - 6] == oppo)) || (plate[temp.x - 6][temp.y - 6] == EMPTY && (plate[temp.x - 1][temp.y - 1] == BLOCK || plate[temp.x - 1][temp.y - 1] == oppo));
			break;
		default:
			break;
	}
}

void sixthStoneBot(char plate[][PLATE_MAX], cord2D *next, cord2D *before, int doNext, int * weightList, int turn, int nowTurn)	{
	// Main AI. Return at next[2].
	// Make candidate proper number ( 50 );
	cord2D oppoCandCord[PLATE_MAX * PLATE_MAX], myCandCord[PLATE_MAX * PLATE_MAX];
	cord2D temp;
	char tempPlate[PLATE_MAX][PLATE_MAX], mPlate[PLATE_MAX][PLATE_MAX], oPlate[PLATE_MAX][PLATE_MAX];
	int oppoCandNum = 0;
	int myCandNum = 0;
	int i, oppo, index[2], loseDir, count = 0;
	int candidateWeight[PLATE_MAX][PLATE_MAX];
	long long int oppoWeight, myWeight, highestWeight = 0;

	// turn mean my color, oppo mean other`s color.
	if(turn == BLACK) oppo = WHITE;
	else oppo = BLACK;

	next[0].x = -1;	next[0].y = -1;
	next[1].x = -1;	next[1].y = -1;
	temp.x = -1;	temp.y = -1;
	index[0] = 0;	index[1] = 0;
	myWeight = 0;

	// Init temp - plate.
	changeBlocking(plate, mPlate, turn);
	changeBlocking(plate, oPlate, oppo);

	if (doNext == 1) {
		// Do only 1.
		// Calculate opposite turn`s highest plate.
		getCandidate(plate, candidateWeight, myCandCord, before, CAND_MAX, weightList, turn);//to get candidateWeight
		minMax(plate, &next[0], temp, 4, -INT_MAX, INT_MAX, true, before, candidateWeight, weightList, turn, 2, turn, true);
		put(plate, next[0], turn);
		printf("(%d, %d)", next[0].x, next[0].y);
		system("pause");

		return ;
	}

	//Search in case we can win.
	if((loseDir = getWinState(mPlate, &temp, turn)) != NO) {
		doWin(plate, temp, loseDir, next, turn);
	}
	else if((loseDir = getWinState(oPlate, &temp, oppo)) != NO){
		doSheild(plate, temp, loseDir, &next[0], turn);
	}

	// Init temp - plate.
	changeBlocking(plate, mPlate, turn);
	changeBlocking(plate, oPlate, oppo);

	if (next[1].x == -1 && next[0].x != -1) {
		// Check if is there any other lose state.
		if ((loseDir = getWinState(oPlate, &temp, oppo)) != NO) {
			// In this case, next[0] will using twice.
			doSheild(plate, temp, loseDir, &next[1], turn);
		}
		else {
			getCandidate(plate, candidateWeight, myCandCord, before, CAND_MAX, weightList, turn);//to get candidateWeight
			minMax(plate, &next[1], temp, DEPTH_MAX - 1, -INT_MAX, INT_MAX, true, before, candidateWeight, weightList, turn, 1, turn, true);
			put(plate, next[1], turn);
		}
	}
	else if(next[1].x == -1 && next[0].x == -1 ){	
		getCandidate(plate, candidateWeight, myCandCord, before, CAND_MAX, weightList, turn);//to get candidateWeight
		minMax(plate, &next[0], temp, DEPTH_MAX, -INT_MAX, INT_MAX, true, before, candidateWeight, weightList, turn, 2, turn, true);
		put(plate, next[0], turn);
		printf("(%d, %d)", next[0].x, next[0].y);
		system("pause");

		temp.x = 0;
		temp.y = 0;
		getCandidate(plate, candidateWeight, myCandCord, before, CAND_MAX, weightList, turn);//to get candidateWeight
		minMax(plate, &next[1], temp, DEPTH_MAX - 1, -INT_MAX, INT_MAX, true, before, candidateWeight, weightList, turn, 1, turn, true);
		put(plate, next[1], turn);
		printf("(%d, %d) \n", next[1].x, next[1].y);
		system("pause");
	}
}
