#include "sixthStone.h"
void initMemorizedCord(memorizedCord2D *memorizedCord) {
	memorizedCord->mine[0].x = -1;
	memorizedCord->mine[0].y = -1;
	memorizedCord->mine[1].x = -1;
	memorizedCord->mine[1].y = -1;
	memorizedCord->opposite[0].x = -1;
	memorizedCord->opposite[0].x = -1;
	memorizedCord->opposite[1].y = -1;
	memorizedCord->opposite[1].y = -1;

}

int main()	{
	char plate[PLATE_MAX][PLATE_MAX];
	int turn = WHITE;
	int i, blockNum;
	cord2D lenCord;
	memorizedCord2D memorizedCord;
	initMemorizedCord(&memorizedCord);

	srand(time(NULL));
	lenCord.x = PLATE_MAX;
	lenCord.y = PLATE_MAX;
	
	blockNum = 5;

	initPlate(plate, blockNum);

	do {
		turn = changeTurn(turn, &memorizedCord);
		display(plate);
		if(turn == WHITE)	{
			sixStoneBot(plate, lenCord, &memorizedCord, turn);
		}
		else {
			for(i = 0; i < 2; i++)	{
				scanf(" %d %d", &memorizedCord.mine[i].x, &memorizedCord.mine[i].y);
				while(canPut(plate, memorizedCord.mine[i], turn) == NO)	{
					printf("%c[1;%dm", 27, 31);
					printf("You can`t put there.\n");
					printf("%c[0m",27);
					scanf(" %d %d", &memorizedCord.mine[0], &memorizedCord.mine[1]);
				}
				put(plate, memorizedCord.mine[i], turn);
				display(plate);
			}
		}
	}	while(whoWin(plate, lenCord, &memorizedCord.mine, turn) == NO);
	display(plate);
	if(turn == WHITE) printf("White Win!\n");
	else printf("Black Win!\n");
	system("pause");

	return 0;
}
