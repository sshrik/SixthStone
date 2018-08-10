// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.

3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.

4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.



[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)


<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/

#include <stdio.h>
#include "Connect6Algo.h"

#include <stdlib.h>

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:SixthStone,Department:CAU" };

cord2D next[2] = { { -1, -1 }, { -1, -1 } }, before[2] = { {0x10, 0x10}, {0x10, 0x10} };

int nowTurn = 1;

char myColor = 0;
char oppoColor = 0;

void myturn(int cnt) {

	int x[2], y[2];
	int i, j;
	char plate[PLATE_MAX][PLATE_MAX];
	int weightListB[20] = { 1, 700, 300, 500, 900, -1, -500, -200, -300, -500, 1, 3, 700, 500, 900, -1, -3, -200, -300, -500 };
	
	// set my collor
	if (myColor == 0 && cnt == 1){
		myColor = BLACK;
		oppoColor = WHITE;
		nowTurn = 1;
	}
	else if (myColor == 0 && cnt == 2){
		myColor = WHITE;
		oppoColor = BLACK;
		nowTurn = 2;
	}

	// make plate
	for (i = 0; i < PLATE_MAX; i++){
		for (j = 0; j < PLATE_MAX; j++){
			if (isFree(i, j)) {
				plate[i][j] = EMPTY;
				continue;
			}
			switch (showBoard(i, j)){
			case 1 :
				plate[i][j] = myColor;
				break;
			case 2:
				plate[i][j] = oppoColor;
				break;
			case 3:
				plate[i][j] = BLOCK;
				break;
			}
		}
	}

	//writeLog("myturn break");
	// calculate
	sixthStoneBot(plate, next, before, cnt, weightListB, myColor, nowTurn);
	nowTurn += 2;

	for (i = 0; i < 2; i++){
		before[i].x = next[i].x;
		before[i].y = next[i].y;
	}
	
	char debugBuffer[200] = {0, };
	sprintf(debugBuffer, "%c, found it : (%d, %d) (%d, %d)\n", myColor, next[0].x, next[0].y, next[1].x, next[1].y);
	writeLog(debugBuffer);
	

	// translate cord2D to array of x,y
	for (i = 0; i < cnt; i++){
		x[i] = next[i].x;
		y[i] = next[i].y;
	}


	domymove(x, y, cnt);
}

// ######################################################################################
//
//									sixStoneAI.c
//
// ######################################################################################

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
	/*
	char debugBuff[200];
	sprintf(debugBuff, "getCandidate:: init\n");
	//writeLog(debugBuff);
	*/
	

	if (count > WINSTATE_MAX) {
		memcpy(realWeightList, &weightList[10], sizeof(int) * 10);
	}
	else {
		memcpy(realWeightList, &weightList[0], sizeof(int) * 10);
	}

	// Add weight for winning state.
	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++)	{
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
	//writeLog("getCandidate:: finish first for\n");

	// Add weight for around state.
	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp.x = i;
			temp.y = j;
			if (i < 5 || i>14 || j < 5 || j>14) {
				candidateWeight[i][j] -= 2;//tempNumber
			}
			if (canPut(plate, temp, turn) == YES) {
				candidateWeight[i][j] += getCandWeight(plate, temp, turn);
			}
		}
	}
	//writeLog("getCandidate:: finish 2nd for\n");

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
				else if (candWeightList[k] < candidateWeight[i][j]){
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
	//writeLog("getCandidate:: finish 3rd for\n");

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
	//writeLog("getCandidate:: finish 4th for\n");

	for (i = 1; i < candNum; i++) {
		if (candWeightList[i] * 100 < candWeightList[0] * 80) {
			memmove(&candCord[i], &candCord[i + 1], sizeof(cord2D) * (candLimit - i - 1));
			memmove(&candWeightList[i], &candWeightList[i + 1], sizeof(int) * (candLimit - i));
			memmove(&candLengthList[i], &candLengthList[i + 1], sizeof(int) * (candLimit - i));
			i--;
			candNum--;
		}
	}
	//writeLog("getCandidate:: finish 5th for\n");


	free(candLengthList);
	free(candWeightList);

	return candNum;
}

int getCandWeight(char plate[][PLATE_MAX], cord2D temp, int turn)	{
	int weight = 0;
	int w, b, i, dir;
	w = 0; b = 0;

	if (!isOutOfPlate(temp, 2, EAST))	{
		if (plate[temp.x][temp.y + 1] == WHITE) w++;
		else if (plate[temp.x][temp.y + 1] == BLACK) b++;
	}
	if (!isOutOfPlate(temp, 2, WEST))	{
		if (plate[temp.x][temp.y - 1] == WHITE) w++;
		else if (plate[temp.x][temp.y - 1] == BLACK) b++;
	}
	if (!isOutOfPlate(temp, 2, SOUTH))	{
		if (plate[temp.x + 1][temp.y] == WHITE) w++;
		else if (plate[temp.x + 1][temp.y] == BLACK) b++;
	}
	if (!isOutOfPlate(temp, 2, NORTH))	{
		if (plate[temp.x - 1][temp.y] == WHITE) w++;
		else if (plate[temp.x - 1][temp.y] == BLACK) b++;
	}

	if (!isOutOfPlate(temp, 2, EAST_SOUTH))	{
		if (plate[temp.x + 1][temp.y] + 1 == WHITE) w++;
		else if (plate[temp.x + 1][temp.y + 1] == BLACK) b++;
	}
	if (!isOutOfPlate(temp, 2, EAST_NORTH))	{
		if (plate[temp.x - 1][temp.y + 1] == WHITE) w++;
		else if (plate[temp.x - 1][temp.y + 1] == BLACK) b++;
	}
	if (!isOutOfPlate(temp, 2, WEST_SOUTH))	{
		if (plate[temp.x + 1][temp.y - 1] == WHITE) w++;
		else if (plate[temp.x + 1][temp.y - 1] == BLACK) b++;
	}
	if (!isOutOfPlate(temp, 2, WEST_NORTH))	{
		if (plate[temp.x - 1][temp.y - 1] == WHITE) w++;
		else if (plate[temp.x - 1][temp.y - 1] == BLACK) b++;
	}

	if (turn == WHITE)	{
		for (i = 1; i <= w; i++)	{
			weight += i;
		}
		for (i = 1; i <= b; i++)	{
			weight -= i;
		}
	}
	else if (turn == BLACK)	{
		for (i = 1; i <= w; i++)	{
			weight -= i;
		}
		for (i = 1; i <= b; i++)	{
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

	for (i = 0; i < PLATE_MAX; i++)	{
		for (j = 0; j < PLATE_MAX; j++)	{
			for (k = 1; k <= 8; k++)	{
				temp.x = i;
				temp.y = j;
				if (isWinState(plate, temp, turn, k, 4) == YES || isWinState(plate, temp, turn, k, 5) == YES)	{
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

void doWin(char plate[][PLATE_MAX], cord2D temp, int dir, cord2D * next, int turn)	{
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
			put(plate, *next, turn);
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

	if (next->x != -1)	put(plate, *next, turn);
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
	if (turn == BLACK) oppo = WHITE;
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
		return;
	}

	//Search in case we can win.
	if ((loseDir = getWinState(mPlate, &temp, turn)) != NO) {
		doWin(plate, temp, loseDir, next, turn);
		//writeLog("win phase");
	}
	else if ((loseDir = getWinState(oPlate, &temp, oppo)) != NO){
		doSheild(plate, temp, loseDir, &next[0], turn);
		//writeLog("lose phase");
	}

	// Init temp - plate.
	changeBlocking(plate, mPlate, turn);
	changeBlocking(plate, oPlate, oppo);

	if (next[1].x == -1 && next[0].x != -1) {
		//writeLog("i already use 1st ball");
		// Check if is there any other lose state.
		if ((loseDir = getWinState(oPlate, &temp, oppo)) != NO) {
			// In this case, next[0] will using twice.
			doSheild(plate, temp, loseDir, &next[1], turn);
			//writeLog("lose phase with 2nd ball");
		}
		else {
			getCandidate(plate, candidateWeight, myCandCord, before, CAND_MAX, weightList, turn);//to get candidateWeight
			minMax(plate, &next[1], temp, DEPTH_MAX - 1, -INT_MAX, INT_MAX, true, before, candidateWeight, weightList, turn, 1, turn, true);
			put(plate, next[1], turn);
			//writeLog("found candidates");
		}
	}
	else if (next[1].x == -1 && next[0].x == -1){
		//writeLog("attempts to find candidate\n");
		getCandidate(plate, candidateWeight, myCandCord, before, CAND_MAX, weightList, turn);//to get candidateWeight
		minMax(plate, &next[0], temp, DEPTH_MAX, -INT_MAX, INT_MAX, true, before, candidateWeight, weightList, turn, 2, turn, true);
		put(plate, next[0], turn);
		//printf("(%d, %d)", next[0].x, next[0].y);
		//system("pause");
		//writeLog("found candidate of 1st\n");
		temp.x = 0;
		temp.y = 0;
		getCandidate(plate, candidateWeight, myCandCord, before, CAND_MAX, weightList, turn);//to get candidateWeight
		minMax(plate, &next[1], temp, DEPTH_MAX - 1, -INT_MAX, INT_MAX, true, before, candidateWeight, weightList, turn, 1, turn, true);
		put(plate, next[1], turn);
		//printf("(%d, %d) \n", next[1].x, next[1].y);
		//system("pause");
		//writeLog("found candidate of 2nd");
	}
}

// ######################################################################################
//
//									sixStoneAPI.c
//
// ######################################################################################

int allWhoWin(char plate[][PLATE_MAX]) {
	int i, j, count = 0;
	cord2D temp[2];
	char cPlate[PLATE_MAX][PLATE_MAX];

	changeBlocking(plate, cPlate, BLACK);

	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp[0].x = i;	temp[1].x = i;
			temp[0].y = j;	temp[1].y = j;
			if (whoWin(cPlate, temp, BLACK) != NO) {
				return cPlate[i][j];
			}
		}
	}

	changeBlocking(plate, cPlate, WHITE);

	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp[0].x = i;	temp[1].x = i;
			temp[0].y = j;	temp[1].y = j;
			if (whoWin(cPlate, temp, WHITE) != NO) {
				return cPlate[i][j];
			}
		}
		if (cPlate[i][j] != EMPTY) count++;
	}
	if (count == PLATE_MAX * PLATE_MAX) return EMPTY;
	return NO;
}

int whoWin(char plate[][PLATE_MAX], cord2D* cord, int turn)	{
	// TODO : Need to add 7-stones to loose.
	char what = turn;
	int c, i;
	char cPlate[PLATE_MAX][PLATE_MAX];

	changeBlocking(plate, cPlate, turn);

	for (c = 0; c < 2; c++)	{
		// East
		if (!isOutOfPlate(cord[c], 6, EAST)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x][cord[c].y + i] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// West
		if (!isOutOfPlate(cord[c], 6, WEST)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x][cord[c].y - i] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// South
		if (!isOutOfPlate(cord[c], 6, SOUTH)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x + i][cord[c].y] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// North
		if (!isOutOfPlate(cord[c], 6, NORTH)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x - i][cord[c].y] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// East-South
		if (!isOutOfPlate(cord[c], 6, EAST_SOUTH)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x + i][cord[c].y + i] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// East-North
		if (!isOutOfPlate(cord[c], 6, EAST_NORTH)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x - i][cord[c].y + i] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}

		// West-South
		if (!isOutOfPlate(cord[c], 6, WEST_SOUTH)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x + i][cord[c].y - i] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}
		// West-Nort
		if (!isOutOfPlate(cord[c], 6, WEST_NORTH)){
			for (i = 0; i < 6; i++)	{
				if (cPlate[cord[c].x - i][cord[c].y - i] != what)	break;
			}
			if (i == 6) return cPlate[cord[c].x][cord[c].y];
		}
	}
	return NO;
}

void put(char plate[][PLATE_MAX], cord2D cord, int what)	{
	plate[cord.x][cord.y] = what;
}

int canPut(char plate[][PLATE_MAX], cord2D cord, int what)	{
	if (isOutOfPlate(cord, 0, 1))	return NO;
	else if (plate[cord.x][cord.y] != EMPTY)	{
		// If plate[x][y] is filled with other stone,
		return NO;
	}
	return YES;
}

void changeTurn(int * turn)	{
	if (*turn == BLACK) *turn = WHITE;
	else *turn = BLACK;
}

void initPlate(char plate[][PLATE_MAX], int blockNum)	{
	int i, j, randNum;
	int blockCount = 0;
	int randomBlockSize = rand() % 6 * 2;
	cord2D temp;

	// Make block( "C" ).
	for (i = 0; i < PLATE_MAX; i++)	{
		for (j = 0; j < PLATE_MAX; j++) {
			plate[i][j] = EMPTY;
		}
	}

	for (int k = 0; k < randomBlockSize; k++) {
		do {
			temp.x = rand() % PLATE_MAX;
			temp.y = rand() % PLATE_MAX;
		} while (plate[temp.x][temp.y] != EMPTY);
		plate[temp.x][temp.y] = BLOCK;
	}
}

int isOutOfPlate(cord2D cord, int next, int dir) {
	// Check x + next or y - next are out of plate.
	switch (dir) {
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
		return cord.x + next - 1 >= PLATE_MAX || cord.y + next - 1 >= PLATE_MAX;
		break;
	case EAST_NORTH:	// X-- Y++
		return cord.x - next + 1 < 0 || cord.y + next - 1 >= PLATE_MAX;
		break;
	case WEST_SOUTH:	// X++ Y--
		return cord.x + next - 1 >= PLATE_MAX || cord.y - next + 1 < 0;
		break;
	case WEST_NORTH:	// X-- Y-- 
		return cord.x - next + 1 < 0 || cord.y - next + 1 < 0;
		break;
	default:
		break;
	}
}

void changeBlocking(char plate[][PLATE_MAX], char changePlate[][PLATE_MAX], int turn) {
	int i, j;
	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			changePlate[i][j] = plate[i][j] == BLOCK ? turn : plate[i][j];
		}
	}
}

int isSevenStone(char plate[][PLATE_MAX], cord2D cord, int turn) {
	// If do at cord location with turn`th turn, does occured seven stone?
	int i, j, k, dir;
	int result = NO;

	cord2D temp;

	plate[cord.x][cord.y] = turn;

	for (j = 0; j < PLATE_MAX; j++) {
		for (k = 0; k < PLATE_MAX; k++) {
			temp.x = j;
			temp.y = k;
			for (dir = 1; dir <= 8; dir++)	{
				isOutOfPlate(temp, 7, dir);
				switch (dir) {
				case EAST:
					for (i = 0; i < 7; i++) {
						if (plate[j][k + i] != turn) break;
					}
					if (i == 8)	result = YES;
					break;
				case WEST:
					for (i = 0; i < 7; i++) {
						if (plate[j][k - i] != turn)  break;
					}
					if (i == 8)	result = YES;
					break;
				case SOUTH:
					for (i = 0; i < 7; i++) {
						if (plate[j + i][k] != turn) break;
					}
					if (i == 8)	result = YES;
					break;
				case NORTH:
					for (i = 0; i < 7; i++) {
						if (plate[j - i][k] != turn)  break;
					}
					if (i == 8)	result = YES;
					break;


				case EAST_SOUTH:
					for (i = 0; i < 7; i++) {
						if (plate[j + i][k + i] != turn) break;
					}
					if (i == 8)	result = YES;
					break;
				case EAST_NORTH:
					for (i = 0; i < 7; i++) {
						if (plate[j - i][k + i] != turn)  break;
					}
					if (i == 8)	result = YES;
					break;
				case WEST_SOUTH:
					for (i = 0; i < 7; i++) {
						if (plate[j + i][k - i] != turn)  break;
					}
					if (i == 8)	result = YES;
					break;
				case WEST_NORTH:
					for (i = 0; i < 7; i++) {
						if (plate[j - i][k - i] != turn)  break;
					}
					if (i == 8)	result = YES;
					break;
				}
			}
		}
	}

	plate[cord.x][cord.y] = EMPTY;

	return result;
}

int getWinningStateNumber(char plate[][PLATE_MAX], int continum, int turn) {
	// Get how many continum winning state.
	int i, j, dir;
	cord2D temp;
	int count = 0;

	for (i = 0; i < PLATE_MAX; i++) {
		for (j = 0; j < PLATE_MAX; j++) {
			temp.x = i;	temp.y = j;
			for (dir = 1; dir <= 8; dir++) {
				if (isWinState(plate, temp, turn, dir, continum) == YES)	count++;
			}
		}
	}

	return count;
}

int doGame(int * weightListW, int * weightListB) {
	int win = NO;
	int turn = BLACK;
	int nowTurn = 1;
	cord2D nextPut[2], before[2];
	char plate[PLATE_MAX][PLATE_MAX];
	int blockNum = 5;

	srand(time(NULL));

	memset(nextPut, -1, sizeof(cord2D) * 2);
	memset(before, 0x09, sizeof(cord2D) * 2);

	initPlate(plate, blockNum);
	sixthStoneBot(plate, nextPut, before, 1, weightListB, turn, nowTurn++);

	do {
		changeTurn(&turn);
		if (turn == WHITE) {
			sixthStoneBot(plate, nextPut, before, 2, weightListW, turn, nowTurn++);
		}
		else {
			sixthStoneBot(plate, nextPut, before, 2, weightListB, turn, nowTurn++);
		}
		memcpy(before, nextPut, sizeof(cord2D) * 2);
		if (allWhoWin(plate) == EMPTY) {
			win == EMPTY;
			break;
		}
	} while (allWhoWin(plate) == NO);

	if (win == EMPTY) {
		// Do nothing.
	}
	else if (turn == WHITE) {
		win = WHITE;
	}
	else if (turn == BLACK){
		win = BLACK;
	}

	return win;
}

// ######################################################################################
//
//									minMaxTree.c
//
// ######################################################################################

void copyMap(char copyPlate[][PLATE_MAX], char originalPlate[][PLATE_MAX]) {
	// -> can change with memcpy(copyPlate, originalPlate, sizeof(char) * PLATE_MAX * PLATE_MAX);
	for (int i = 0; i < PLATE_MAX; i++) {
		for (int j = 0; j < PLATE_MAX; j++) {
			copyPlate[i][j] = originalPlate[i][j];
		}
	}
}

int minMax(char tempPlate[][PLATE_MAX], cord2D *next, cord2D cord, int depth, int alpha, int beta, bool isMaximizingPlayer, cord2D *before, int tempCandidateWeight[][PLATE_MAX], int *weightList, int turnInTree, int turnCount, int myRealTurn, bool isRoot) {
	//writeLog("minMax:: called minMax\n");
	int maxValue = -9999, minValue = 9999;//temporary value
	int temp, candidateNum = 0, loseDir, oppoTurn, rootMax = -99999;
	cord2D selectedCord;
	cord2D candCord[CAND_MAX * 2], tempCord[CAND_MAX * 2];//temporary array size
	char savedPlate[PLATE_MAX][PLATE_MAX], changedPlate[PLATE_MAX][PLATE_MAX]; //original plate which stored at this depth's method.
	oppoTurn = turnInTree == WHITE ? BLACK : WHITE;
	copyMap(savedPlate, tempPlate);
	//writeLog("minMax:: init\n");

	if (--turnCount < 0) {
		isMaximizingPlayer = isMaximizingPlayer ? false : true;
		turnInTree = oppoTurn;
		oppoTurn = turnInTree == WHITE ? BLACK : WHITE;
		turnCount = 1;
	}
	
	changeBlocking(tempPlate, changedPlate, turnInTree);
	if (depth == 0) {
		//writeLog("minMax:: depth 0\n");
		changeBlocking(tempPlate, changedPlate, myRealTurn);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, myRealTurn); //myRealTurn에 대한 tempCandidateWeight구하기
		int weight = calcWeight(tempCandidateWeight);
		//writeLog("minMax:: depth 0 return\n");
		return weight;
	}
	//Search in case we can win.(내 턴이 2개 남았을 때)
	if ((turnCount > 0) && (loseDir = getWinState(changedPlate, &cord, turnInTree) != NO)) {
		if (turnInTree == myRealTurn) {
			//writeLog("minMax:: win case\n");
			return WIN_WEIGHT;
		}
		else {
			//printf("상대가 (%d, %d)여기서 찾을 수 있는 방향의 공격지점에 둬서 내가 졌을 때 - depth : %d\n", cord.x, cord.y, depth);
			//writeLog("minMax:: lose case\n");
			return -WIN_WEIGHT;
		}
	}
	//writeLog("minMax:: breakpoint-1\n");
	//Search in case we would loose(내 턴이 몇개 남았든 상관 없음)
	changeBlocking(tempPlate, changedPlate, oppoTurn);
	if ((loseDir = getWinState(changedPlate, &cord, oppoTurn)) != NO) {
		//writeLog("minMax:: breakpoint-1.1\n");
		doSheild(changedPlate, cord, loseDir, &candCord[0], turnInTree);
		put(tempPlate, candCord[0], turnInTree);//tempPlate에 주어진 cord자리에 바둑돌 놓기
		//writeLog("minMax:: lose position found\n");
		temp = minMax(tempPlate, next, candCord[0], depth - 1, alpha, beta, isMaximizingPlayer, before, tempCandidateWeight, weightList, turnInTree, turnCount, myRealTurn, false);

		if (isRoot) {
			next->x = candCord[0].x;
			next->y = candCord[0].y;
		}
		return temp;
	}
	else {
		//writeLog("minMax:: breakpoint-1.2\n");
		changeBlocking(tempPlate, changedPlate, turnInTree);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, candCord, before, CAND_MAX, weightList, turnInTree); //다음 바둑돌 위치 찾기
		changeBlocking(tempPlate, changedPlate, myRealTurn);
		candidateNum = getCandidate(changedPlate, tempCandidateWeight, tempCord, before, CAND_MAX, weightList, turnInTree); //myRealTurn 입장에서의 weight구하기
		changeBlocking(tempPlate, changedPlate, turnInTree);//다시 되돌리기

	}
	//writeLog("minMax:: breakpoint-2\n");
	//win state 경우 찾은 후 해당 자리에 candCord좌표 설정
	for (int i = 0; i < candidateNum; i++) {
		put(tempPlate, candCord[i], turnInTree);//tempPlate에 주어진 cord자리에 바둑돌 놓기
		//writeLog("minMax:: win position found\n");
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
	//writeLog("minMax:: breakpoint-3\n");
	return isMaximizingPlayer ? maxValue : minValue;
}