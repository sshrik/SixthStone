#include "genetic.h"
#include "sixthStone.h"

void initGen(int gen[][GEN_LENGTH]) {
	int i, j, k;
	int x, y, temp;
	int randNum[GEN_LENGTH / 4];

	for (i = 0; i < GENERATION_MAX; i++) {
		for (j = 0; j < 4; j++) {
			// Make 5 and sort.
			for (k = 0; k < GEN_LENGTH / 4; k++) {
				randNum[k] = rand() % GEN_MAX;
			}
			for (x = 0; x < GEN_LENGTH / 4; x++) {
				for (y = x + 1; y < GEN_LENGTH / 4; y++) {
					if (randNum[x] > randNum[y]) {
						temp = randNum[y];
						randNum[y] = randNum[x];
						randNum[x] = temp;
					}
				}
			}
			// Then copy to weightList. weightList must be in order with increase.
			memcpy(&gen[i][j * (GEN_LENGTH / 4)], randNum, sizeof(int) * GEN_LENGTH / 4);
		}
	}
}

void makeChild(int par[][GEN_LENGTH], int child[][GEN_LENGTH]) {
	// Make par * par pair and make child with each par * CHILD_NUM.
	int i, j, k;
	int childLoc;

	for (i = 0; i < PARENT_MAX; i++) {
		for (j = 0; j < PARENT_MAX; j++) {
			for (k = 0; k < CHILD_NUM; k++) {
				childLoc = rand() % GEN_LENGTH;
				memcpy(child[i * PARENT_MAX + j * CHILD_NUM + k], par[i], sizeof(int) * (childLoc));
				memcpy(&child[i * PARENT_MAX + j * CHILD_NUM + k][childLoc], &par[j][childLoc], sizeof(int) * (GEN_LENGTH - childLoc));
			}
		}
	}
}

int calcProfit(int * child, int * dst) {
	// Calc one child`s profit to dst.
	int i, result = 0;

	for (i = 0; i < GEN_LENGTH; i++) {
		result += (child[i] - dst[i]) * (child[i] - dst[i]);
	}

	return result / GEN_LENGTH;
}

void makeMutate(int * gen) {
	// Make Mutation at gen.
	int mutPercent = rand() % 100;
	int mutLocation, i;

	if (mutPercent > MUT_PER) {
		mutLocation = rand() % GEN_LENGTH;
		for (i = mutLocation; i < GEN_LENGTH; i++) {
			if ((i + 1) % 5 == 0) {
				gen[i] = gen[i] + rand() % (GEN_MAX - gen[i]);
			}
			else gen[i] = gen[i] + rand() % (gen[i + 1] - gen[i]);
		}
	}
}

void choosePar(int gen[][GEN_LENGTH], int par[][GEN_LENGTH]) {
	// Calc with winning state.
	/*
		1. choose higest PARENT_MAX / 2 number of state.
		2. choose opposite highest winning rate.
		3. if same weightList is choosed, mutate or change to new one.
	*/
	int i, j, k, game, temp, win;
	int logB = 0;
	int winPlate[GENERATION_MAX][GENERATION_MAX];
	// winPlate[i][j] mean weightList[i]`s ( == gen[i]`s ) win game number oppose to weightList[j] ( == gen[j]`s ).
	float genProfit[GENERATION_MAX];
	int index[GENERATION_MAX];
	char log[5000];

	memset(winPlate, 0x00, sizeof(int) * GENERATION_MAX * GENERATION_MAX);

	for (i = 0; i < GENERATION_MAX; i++) {
		for (j = 0; j < GENERATION_MAX; j++) {
			if (i != j) {
				printf("Game %d vs %d proceeding.\n", i, j);
				printf("WeigthList[%d] : ", i);
				for (k = 0; k < GEN_LENGTH; k++) {
					printf("%d ", gen[i][k]);
				}
				printf("\n");
				printf("WeigthList[%d] : ", j);
				for (k = 0; k < GEN_LENGTH; k++) {
					printf("%d ", gen[j][k]);
				}
				printf("\n");
				for(game = 0; game < GAME_MAX; game++)	printf("#");
				printf("\n");
				for (game = 0; game < GAME_MAX; game++) {
					printf("#");
					win = doGame(gen[i], gen[j]);
					if (win == WHITE) winPlate[i][j]++;
					else if(win == BLACK) winPlate[j][i]++;
				}
				printf("\n");
			}
		}
	}

	printf("Getting winning count...\n");
	for (i = 0; i < GENERATION_MAX; i++) {
		genProfit[i] = getWinRate(winPlate, i);
		index[i] = i;
	}

	for (i = 0; i < GENERATION_MAX; i++) {
		for (j = i + 1; j < GENERATION_MAX; j++) {
			if (genProfit[i] < genProfit[j]) {
				temp = genProfit[i];
				genProfit[i] = genProfit[j];
				genProfit[j] = temp;

				temp = index[i];
				index[i] = index[j];
				index[j] = temp;
			}
		}
	}

	printf("Getting copying now step1...\n");
	for (i = 0; i < PARENT_MAX / 2; i++) {
		memcpy(par[i], gen[index[i]], sizeof(int) * GEN_LENGTH);
	}
	k = 0;

	printf("Getting copying now step2...\n");
	for (i = PARENT_MAX / 2; i < PARENT_MAX; i++) {
		if ((temp = getOppoIndex(winPlate, i - (PARENT_MAX / 2), 60)) != -1) {
			memcpy(par[i], gen[temp], sizeof(int) * GEN_LENGTH);
		}
		else {
			memcpy(par[i + k], gen[index[i + k]], sizeof(int) * GEN_LENGTH);
			k++;
		}
	}

	for (i = 0; i < PARENT_MAX; i++) {
		// Init for log.
		memset(log, 0x00, sizeof(char) * 5000);
		logB = 0;

		printf("WeigthList[%d] : ", i);
		logB += sprintf(log + logB, "WeigthList[%d] : ", i);

		for (k = 0; k < GEN_LENGTH; k++) {
			printf("%d ", par[i][k]);
			logB += sprintf(log + logB, "%d ", par[i][k]);
		}

		printf("// Win Rate [ %f ]\n", genProfit[i]);
		logB += sprintf(log + logB, "// Win Rate [ %f ]\n", genProfit[i]);

		makeLog(log);
	}
}

float getWinRate(int winPlate[][GENERATION_MAX], int i) {
	int x, winGame = 0;

	// Calc all win game number.
	for (x = 0; x < GENERATION_MAX; x++) {
		winGame += winPlate[i][x];
	}
	return ((float)winGame * (float)100 ) / ((float)2 * (float)GAME_MAX * (float)(GENERATION_MAX - 1) );
}

int getOppoIndex(int winPlate[][GENERATION_MAX], int i, int rate) {
	// Get highest win rate opponent to index i.
	int x, highestWin = winPlate[i][0];
	int returnIndex = 0;

	for (x = 1; x < GENERATION_MAX; x++) {
		if (winPlate[i][x] > highestWin) {
			highestWin = winPlate[i][x];
		}
	}

	// If not over win-rate 60%, drop it.
	if (highestWin * 100 < GAME_MAX * rate) {
		returnIndex = -1;
	}

	return returnIndex;
}

int findProfit(int child[][GEN_LENGTH], int * dst, int profitDst) {
	int i;

	for (i = 0; i < GENERATION_MAX; i++) {
		if (calcProfit(child[i], dst) < profitDst) {
			return i;
		}
	}

	return -1;
}

void makeLog(char * str) {
	FILE * fp = fopen("genlog.txt", "a");

	fprintf(fp, "%s", str);

	fclose(fp);
}