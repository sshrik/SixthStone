#include "sixthStone.h"


void copyMap(char copyPlate[][PLATE_MAX], char originalPlate[][PLATE_MAX]) {
	// -> can change with memcpy(copyPlate, originalPlate, sizeof(char) * PLATE_MAX * PLATE_MAX);
	for (int i = 0; i < PLATE_MAX; i++) {
		for (int j = 0; j < PLATE_MAX; j++) {
			copyPlate[i][j] = originalPlate[i][j];
		}
	}
}

int minMax(char tempPlate[][PLATE_MAX], cord2D *next, cord2D cord, int depth, int alpha, int beta, bool isMaximizingPlayer, cord2D *before, int tempCandidateWeight[][PLATE_MAX], int *weightList, int turnInTree, int turnCount, int myRealTurn, bool isRoot) {
	int maxValue = -9999, minValue = 9999;//temporary value
	int temp, candidateNum;
	cord2D selectedCord;
	cord2D candCord[CAND_MAX * 2];//temporary array size
	char savedPlate[PLATE_MAX][PLATE_MAX], changedPlate[PLATE_MAX][PLATE_MAX]; //original plate which stored at this depth's method.
	copyMap(savedPlate, tempPlate);

	if (--turnCount < 0) {
		isMaximizingPlayer = isMaximizingPlayer ? false : true;
		turnInTree = turnInTree == WHITE ? BLACK : WHITE;
		turnCount = 1;
		cord.x = -1;
		cord.y = -1;
	}

	changeBlocking(tempPlate, changedPlate, turnInTree);

	if (depth == 0 || (turnCount == 1 && getWinState(changedPlate, &cord, turnInTree) != NO)) {
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, myRealTurn); //myRealTurn에 대한 tempCandidateWeight구하기
		int weight = calcWeight(tempCandidateWeight);
		return weight;
	}

	candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, turnInTree); //다음 바둑돌 위치 찾기
	//win state 경우 찾은 후 해당 자리에 candCord좌표 설정
	for (int i = 0; i < candidateNum; i++) {
		put(tempPlate, candCord[i], turnInTree);//tempPlate에 주어진 cord자리에 바둑돌 놓기
		temp = minMax(tempPlate, next, candCord[i], depth - 1, alpha, beta, isMaximizingPlayer, before, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);
		copyMap(tempPlate, savedPlate);
		if (isMaximizingPlayer) {
			alpha = max(alpha, temp);
			if (beta <= alpha) {
				break;
			}
			if (maxValue < temp) {
				maxValue = temp;
				selectedCord = candCord[i];
			}
		}
		else {
			beta = min(beta, temp);
			if (alpha <= beta) {
				break;
			}
			if (minValue > temp) {
				minValue = temp;
				selectedCord = candCord[i];
			}
		}

		if (isRoot) {
			next->x = selectedCord.x;
			next->y = selectedCord.y;
		}
	}
	return isMaximizingPlayer ? maxValue : minValue;
}