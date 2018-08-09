#include "sixthStone.h"

int alpha = -9999, beta = 9999;

void copyMap(char copyPlate[][PLATE_MAX], char originalPlate[][PLATE_MAX]) {
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
	cord2D tempCord[5];//for use before.....????????
	char savedPlate[PLATE_MAX][PLATE_MAX]; //original plate which stored at this depth's method.
	copyMap(savedPlate, tempPlate);

	if (--turnCount < 0) {
		isMaximizingPlayer = isMaximizingPlayer ? false : true;
		turnInTree = turnInTree == WHITE ? BLACK : WHITE;
		turnCount = 1;
		cord.x = -1;
		cord.y = -1;
	}

	if (depth == 0 || (turnCount == 1 && getWinState(tempPlate, &cord, turnInTree) != NO)) {
		printf("depth : %d, dir : %d\n", depth, getWinState(tempPlate, &cord, turnInTree));

		candidateNum = getCandidate(tempPlate, tempCandidateWeight, candCord, tempCord, 2, weightList, myRealTurn); //myRealTurn�� ���� tempCandidateWeight���ϱ�
		int weight = calcWeight(tempCandidateWeight);
		printf("weight : %d\n", weight);
		//system("pause");
		return weight;
	}

	candidateNum = getCandidate(tempPlate, tempCandidateWeight, candCord, tempCord, 2, weightList, turnInTree); //���� �ٵϵ� ��ġ ã��
	//win state ��� ã�� �� �ش� �ڸ��� candCord��ǥ ����
	for (int i = 0; i < candidateNum; i++) {
		put(tempPlate, candCord[i], turnInTree);//tempPlate�� �־��� cord�ڸ��� �ٵϵ� ����
		display(tempPlate);
		printf("%c_candidateNum : %d\n", turnInTree, candidateNum);
		printf("��ǥ : (%d,%d), depth : %d\n", candCord[i].x, candCord[i].y, depth);
		//system("pause");
		temp = minMax(tempPlate, next, candCord[i], depth - 1, isMaximizingPlayer, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);
		copyMap(tempPlate, savedPlate);
		if (isMaximizingPlayer) {
			//alpha = max(alpha, temp);
			//if (beta <= alpha) {
			//	break;
			//}
			if (maxValue < temp) {
				maxValue = temp;
				selectedCord = candCord[i];
			}
		}
		else {
			//beta = min(beta, temp);
			//if (alpha <= beta) {
			//	break;
			//}
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