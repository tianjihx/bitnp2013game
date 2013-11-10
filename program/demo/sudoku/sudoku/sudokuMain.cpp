
#include "process.h"
#include "sudokuGameLogic.h"

int pad[9][9];
int padConstFlag[9][9];
int padAnswer[9][9];
int cursorX = 0;
int cursorY = 0;
GameState gameState;
int runningTime = 0;
int timeAgo, timeNow;//用来记录上一次的时间，和当前的时间




int main()
{
	system("color F0");//设置控制台窗口的背景颜色为F（白色），文字颜色为0（黑色）
	int i,j;
	char keyType;//用来存放按键值
	DrawPad();//sudokuDraw.h中的函数，用来绘制数独格子
	DrawTime(gameState,0);//sudokuDraw.h中的函数，绘制时间，传递参数状态gameState,和时间0
	InitialAndDrawGame(pad,padAnswer,padConstFlag);//sudokuDraw.h中的函数，初始化游戏，并且将初始化后的数独游戏填进去
	gameState = Running;//将游戏状态改为Running
	timeAgo = GetTime();//记录时间
	_beginthread(KeyCheckThread,0,NULL);//建立一个线程，专门用来获取键盘动作。KeyCheckThread是在sudokuGameLogic.h中的函数。其他参数都是格式要求，可以不管
	while(1)
	{
		GameRun();//游戏开跑！！这是在sudokuGameLogic.h中的函数。用处：统筹全局代码运行
	}
	return 0;

}