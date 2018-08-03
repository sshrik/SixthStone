#include "sixthStone.h"
void initMemorizedCord(memorizedCord2D *memorizedCord) {
	memset(memorizedCord, -1, sizeof(memorizedCord2D));
}

int main() {
	char plate[PLATE_MAX][PLATE_MAX];
	int turn = WHITE;
	int i, blockNum;
	cord2D lenCord;
	memorizedCord2D memorizedCord;
	initMemorizedCord(&memorizedCord);

	srand(time(NULL));

	blockNum = 5;

	initPlate(plate, blockNum);

	do {
		changeTurn(&turn, &memorizedCord);
		display(plate);
		if (turn == WHITE) {
			sixStoneBot(plate, &memorizedCord, turn);
		}
		else {
			for (i = 0; i < 2; i++) {
				scanf(" %d %d", &memorizedCord.mine[i].x, &memorizedCord.mine[i].y);
				while (canPut(plate, memorizedCord.mine[i], turn) == NO) {
					printf("%c[1;%dm", 27, 31);
					printf("You can`t put there.\n");
					printf("%c[0m", 27);
					scanf(" %d %d", &memorizedCord.mine[0].x, &memorizedCord.mine[1].y);
				}
				put(plate, memorizedCord.mine[i], turn);
				display(plate);
			}
			for (i = 0; i < 2; i++) {
				printf("Do at [%d][%d]\n", memorizedCord.mine[i].x, memorizedCord.mine[i].y);
				system("pause");
			}
		}
	} while (whoWin(plate, &memorizedCord.mine, turn) == NO);
	display(plate);
	if(turn == WHITE) printf("White Win!\n");
	else printf("Black Win!\n");
	system("pause");

	return 0;
}
