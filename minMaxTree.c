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
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, myRealTurn); //myRealTurn에 대한 tempCandidateWeight구하기
		int weight = calcWeight(tempCandidateWeight);
		printf("depth가 0일때   candCord[0]:(%d,%d), weight : %d\n", candCord[0].x, candCord[0].y, weight);
		return weight;
	}
	//Search in case we can win.(내 턴이 2개 남았을 때)
	if ((turnCount > 0) && (loseDir = getWinState(changedPlate, &cord, turnInTree) != NO)) {
		if (turnInTree == myRealTurn) {
			printf("내가 (%d, %d)여기서 찾을 수 있는 방향의 공격지점에 둬서 이겼을 때 - depth : %d\n",cord.x, cord.y, depth);
			return WIN_WEIGHT;
		}
		else {
			printf("상대가 (%d, %d)여기서 찾을 수 있는 방향의 공격지점에 둬서 내가 졌을 때 - depth : %d\n", cord.x, cord.y, depth);
			return -WIN_WEIGHT;
		}
	}
	//Search in case we would loose(내 턴이 몇개 남았든 상관 없음)
	changeBlocking(tempPlate, changedPlate, oppoTurn);
	if ((loseDir = getWinState(changedPlate, &cord, oppoTurn)) != NO) {
		doSheild(changedPlate, cord, loseDir, &candCord[0], turnInTree);
		put(tempPlate, candCord[0], turnInTree);//tempPlate에 주어진 cord자리에 바둑돌 놓기
		printf("depth가 %d 일 때 (%d, %d)에 방어를 했을 때. 내 turn : %c\n", depth, candCord[0].x, candCord[0].y , turnInTree);
		temp = minMax(tempPlate, next, candCord[0], depth - 1, alpha, beta, isMaximizingPlayer, before, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);

		if (isRoot) {
			next->x = candCord[0].x;
			next->y = candCord[0].y;
		}
		return temp;

	}
	else {
		changeBlocking(tempPlate, changedPlate, turnInTree);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, turnInTree); //다음 바둑돌 위치 찾기
		changeBlocking(tempPlate, changedPlate, myRealTurn);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, tempCord, before, CAND_MAX, weightList, turnInTree); //myRealTurn 입장에서의 weight구하기
		changeBlocking(tempPlate, changedPlate, turnInTree);//다시 되돌리기

		for (int i = 0; i < candidateNum; i++) {
			printf("depth가 %d 일 때 후보들 : (%d, %d) %d점 후보 총 갯수 : %d, turn : %c \n", depth, candCord[i].x, candCord[i].y, tempCandidateWeight[candCord[i].x][candCord[i].y], candidateNum, turnInTree);
		}
		printf("--------------------------------------------------------------------------\n");
	}
	//win state 경우 찾은 후 해당 자리에 candCord좌표 설정
	for (int i = 0; i < candidateNum; i++) {
		put(tempPlate, candCord[i], turnInTree);//tempPlate에 주어진 cord자리에 바둑돌 놓기
		temp = minMax(tempPlate, next, candCord[i], depth - 1, alpha, beta, isMaximizingPlayer, before, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);
		printf("depth가 %d 일 때 %d번째 minMax에서 나온 결과 값 : %d, 내 turn : %c\n", depth, i, temp, turnInTree);
		copyMap(tempPlate, savedPlate);
		//printf("40     temp : %d, minmax정보 | candcord(%d,%d), depth : %d,turn : %c\n", temp, candCord[i].x, candCord[i].y, depth, turnInTree);
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
			//printf("70      root : selctedCord(%d,%d), i : %d => %d점\n", selectedCord.x, selectedCord.y, i, maxValue);
			next->x = selectedCord.x;
			next->y = selectedCord.y;
		}
	}
	return isMaximizingPlayer ? maxValue : minValue;
}