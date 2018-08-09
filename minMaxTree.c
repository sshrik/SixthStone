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
	int temp, candidateNum, loseDir, oppoTurn, rootMax = -99999;
	cord2D selectedCord;
	cord2D candCord[CAND_MAX * 2], tempCord[CAND_MAX * 2];//temporary array size
	char savedPlate[PLATE_MAX][PLATE_MAX], changedPlate[PLATE_MAX][PLATE_MAX]; //original plate which stored at this depth's method.
	oppoTurn = turnInTree == WHITE ? BLACK : WHITE;
	copyMap(savedPlate, tempPlate);

	if (--turnCount < 0) {
		isMaximizingPlayer = isMaximizingPlayer ? false : true;
		turnInTree = oppoTurn;
		turnCount = 1;
	}

	changeBlocking(tempPlate, changedPlate, turnInTree);
	if (depth == 0) {
		changeBlocking(tempPlate, changedPlate, myRealTurn);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, myRealTurn); //myRealTurn�� ���� tempCandidateWeight���ϱ�
		int weight = calcWeight(tempCandidateWeight);
		printf("depth�� 0�϶�   candCord[0]:(%d,%d), weight : %d\n", candCord[0].x, candCord[0].y, weight);
		return weight;
	}
	//Search in case we can win.(�� ���� 2�� ������ ��)
	if ((turnCount > 0) && (loseDir = getWinState(changedPlate, &cord, turnInTree) != NO)) {
		if (turnInTree == myRealTurn) {
			printf("���� (%d, %d)���⼭ ã�� �� �ִ� ������ ���������� �ּ� �̰��� �� - depth : %d\n",cord.x, cord.y, depth);
			return WIN_WEIGHT;
		}
		else {
			printf("��밡 (%d, %d)���⼭ ã�� �� �ִ� ������ ���������� �ּ� ���� ���� �� - depth : %d\n", cord.x, cord.y, depth);
			return -WIN_WEIGHT;
		}
	}
	//Search in case we would loose(�� ���� � ���ҵ� ��� ����)
	changeBlocking(tempPlate, changedPlate, oppoTurn);
	if ((loseDir = getWinState(changedPlate, &cord, oppoTurn)) != NO) {
		doSheild(changedPlate, cord, loseDir, &candCord[0], turnInTree);
		put(tempPlate, candCord[0], turnInTree);//tempPlate�� �־��� cord�ڸ��� �ٵϵ� ����
		printf("depth�� %d �� �� (%d, %d)�� �� ���� ��. �� turn : %c\n", depth, candCord[0].x, candCord[0].y , turnInTree);
		temp = minMax(tempPlate, next, candCord[0], depth - 1, alpha, beta, isMaximizingPlayer, before, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);

		if (isRoot) {
			next->x = candCord[0].x;
			next->y = candCord[0].y;
		}
		return temp;

	}
	else {
		changeBlocking(tempPlate, changedPlate, turnInTree);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, turnInTree); //���� �ٵϵ� ��ġ ã��
		changeBlocking(tempPlate, changedPlate, myRealTurn);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, tempCord, before, CAND_MAX, weightList, turnInTree); //myRealTurn ���忡���� weight���ϱ�
		changeBlocking(tempPlate, changedPlate, turnInTree);//�ٽ� �ǵ�����

		for (int i = 0; i < candidateNum; i++) {
			printf("depth�� %d �� �� �ĺ��� : (%d, %d) %d�� �ĺ� �� ���� : %d, turn : %c \n", depth, candCord[i].x, candCord[i].y, tempCandidateWeight[candCord[i].x][candCord[i].y], candidateNum, turnInTree);
		}
		printf("--------------------------------------------------------------------------\n");
	}
	//win state ��� ã�� �� �ش� �ڸ��� candCord��ǥ ����
	for (int i = 0; i < candidateNum; i++) {
		put(tempPlate, candCord[i], turnInTree);//tempPlate�� �־��� cord�ڸ��� �ٵϵ� ����
		temp = minMax(tempPlate, next, candCord[i], depth - 1, alpha, beta, isMaximizingPlayer, before, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);
		printf("depth�� %d �� �� %d��° minMax���� ���� ��� �� : %d, �� turn : %c\n", depth, i, temp, turnInTree);
		copyMap(tempPlate, savedPlate);
		//printf("40     temp : %d, minmax���� | candcord(%d,%d), depth : %d,turn : %c\n", temp, candCord[i].x, candCord[i].y, depth, turnInTree);
		if (isMaximizingPlayer) {
			alpha = max(alpha, temp);
			if (beta <= alpha) {
				break;
			}
			if (maxValue < temp) {
				maxValue = temp;
				selectedCord = candCord[i];
				//printf("50      %d_max_selected : %d,(%d,%d)\n",i, maxValue, selectedCord.x, selectedCord.y);

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
				//printf("60      %d_min_selected : %d,(%d,%d)\n",i, minValue, selectedCord.x, selectedCord.y);
			}
		}

		if (isRoot) {
			//printf("70      root : selctedCord(%d,%d), i : %d => %d��\n", selectedCord.x, selectedCord.y, i, maxValue);
			next->x = selectedCord.x;
			next->y = selectedCord.y;
		}
	}
	return isMaximizingPlayer ? maxValue : minValue;
}