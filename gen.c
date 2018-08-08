#include "genetic.h"

void initGen(int gen[][GEN_LENGTH]) {
	int i, j;

	for (i = 0; i < GENERATION_MAX; i++) {
		for (j = 0; j < GEN_LENGTH; j++) {
			gen[i][j] = rand() % GEN_MAX;
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
			gen[i] = rand() % GEN_MAX;
		}
	}
}

void choosePar(int gen[][GEN_LENGTH], int par[][GEN_LENGTH], int * dst) {
	int i, j, temp;
	int genProfit[GENERATION_MAX], index[GENERATION_MAX];

	for (i = 0; i < GENERATION_MAX; i++) {
		genProfit[i] = calcProfit(gen[i], dst);
		index[i] = i;
	}
	for (i = 0; i < GENERATION_MAX; i++) {
		for (j = i + 1; j < GENERATION_MAX; j++) {
			if (genProfit[i] > genProfit[j]) {
				temp = genProfit[i];
				genProfit[i] = genProfit[j];
				genProfit[j] = temp;

				temp = index[i];
				index[i] = index[j];
				index[j] = temp;
			}
		}
	}

	for (i = 0; i < PARENT_MAX - 5; i++) {
		memcpy(par[i], gen[index[i]], sizeof(int) * GEN_LENGTH);
	}
	for (i = PARENT_MAX - 5; i < PARENT_MAX; i++) {
		for (j = 0; j < GEN_LENGTH; j++) {
			par[i][j] = rand() % GEN_MAX;
		}
	}
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