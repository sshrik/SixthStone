#include "sixthStone.h"

int alpha = -9999, beta = 9999;

void copyMap(char copyPlate[][PLATE_MAX], char originalPlate[][PLATE_MAX]) {
	// -> can change with memcpy(copyPlate, originalPlate, sizeof(char) * PLATE_MAX * PLATE_MAX);
	for (int i = 0; i < PLATE_MAX; i++) {
		for (int j = 0; j < PLATE_MAX; j++) {
			copyPlate[i][j] = originalPlate[i][j];
		}
	}
}

int minMax(char tempPlate[][PLATE_MAX], cord2D *next, cord2D cord, int depth, bool isMaximizingPlayer, int tempCandidateWeight[][PLATE_MAX], int *weightList, int turnInTree, int turnCount, int myRealTurn, bool isRoot) {
	int maxValue = -9999, minValue = 9999;//temporary value
	int temp, candidateNum;
	cord2D selectedCord;
	cord2D candCord[5];//temporary array size
	cord2D tempCord[2];//for use before.....????????
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
	//	printf("depth : %d, dir : %d\n", depth, getWinState(tempPlate, &cord, turnInTree));

		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, tempCord, 2, weightList, myRealTurn); //myRealTurn에 대한 tempCandidateWeight구하기
		int weight = calcWeight(tempCandidateWeight);
	//	printf("weight : %d\n", weight);
		//system("pause");
		return weight;
	}

	candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, tempCord, 2, weightList, turnInTree); //다음 바둑돌 위치 찾기
	//win state 경우 찾은 후 해당 자리에 candCord좌표 설정
	for (int i = 0; i < candidateNum; i++) {
		put(tempPlate, candCord[i], turnInTree);//tempPlate에 주어진 cord자리에 바둑돌 놓기
		//display(tempPlate);
	//	printf("%c_candidateNum : %d\n", turnInTree, candidateNum);
	//	printf("좌표 : (%d,%d), depth : %d\n", candCord[i].x, candCord[i].y, depth);
		//system("pause");
		temp = minMax(tempPlate, next, candCord[i], depth - 1, isMaximizingPlayer, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);
		copyMap(tempPlate, savedPlate);
		if (isMaximizingPlayer) {
			if (maxValue < temp) {
				maxValue = temp;
				selectedCord = candCord[i];
			}
		}
		else {
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