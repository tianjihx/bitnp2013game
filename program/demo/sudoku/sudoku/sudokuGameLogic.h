
#include "sudokuDraw.h"

extern int cursorX;
extern int cursorY;
extern int pad[9][9];
extern int padConstFlag[9][9];
extern int padAnswer[9][9];
extern GameState gameState; 
extern int runningTime;
extern int timeAgo;
extern int timeNow;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77


//按下数字键时触发
void NumberKeyPress(int ascii_code)
{
	int i = cursorY;
	int j = cursorX;
	int numTap = ascii_code - '0';
	if (padConstFlag[i][j] == 0 && gameState == Running)
	{
		int BlockCode = (i/3)*3+(j/3);//将数独数组的坐标(iPad, jPad)变换成小宫格的编号BlockCode.
		if (!CheckNumberExistInBlock(pad,BlockCode, numTap) &&
			!CheckNumberExistInColum(pad,j,numTap) &&
			!CheckNumberExistInRow(pad,i,numTap))
		{
			pad[i][j] = numTap;
			SetColor(blue);
			WriteNumberAt(cursorX,cursorY,numTap);
			if (CheckFinished(pad))
			{
				gameState = Win;
				CursorGotoXYReal(50,1);
				printf("你解对了！              ");
				/*if (highScores[level] > runningTime)
				{
					WriteHighScore(level,runningTime);
					sprintf(buffer,"新纪录！");
					SetTextColor(hdc,RGB(0,0,255));
					TextOut(hdc,560,310,buffer,strlen(buffer));
				}*/
							
			}
		}
	}
	else if (gameState == ComputerPlay)//如果正处于电脑解体
	{
		SetColor(magenta);//设为品红色
		int BlockCode = (i/3)*3+(j/3);//将数独数组的坐标(iPad, jPad)变换成小宫格的编号BlockCode.
		if (!CheckNumberExistInBlock(pad,BlockCode, numTap) &&
			!CheckNumberExistInColum(pad,j,numTap) &&
			!CheckNumberExistInRow(pad,i,numTap))//判断数字是否可以填上去
		{
			pad[i][j] = numTap;//填上去
			padConstFlag[i][j] = 1;//将这个位置设为固定
			WriteNumberAt(cursorX,cursorY,numTap);
		}
	}
	else if (gameState == ComputerPlayEnd )
		return;
}

//按下显示答案键时触发
void AnswerKeyPress()
{
	if (gameState == Running)
	{
		gameState = AnswerShow;
		ShowAnswer(pad,padConstFlag,padAnswer);
		CursorGotoXY(cursorX,cursorY);
	}
}

//按下重新开始键时触发
void RestartKeyPress()
{
	if (gameState == Running)
	{
		gameState = Pause;
		if (QuestionMessageShowFixPosition("放弃重新开始？    ","键入y表示是，任意键表示否",red) == 0)
		{
			gameState = Running;
			return;
		}

	}
	CursorGotoXYReal(45,1);
	printf("                           ");
	runningTime = 0;
	DrawTime(Running,0);//sudokuDraw.h中的函数，绘制时间，传递参数状态gameState,和时间0
	InitialAndDrawGame(pad,padAnswer,padConstFlag);
	gameState = Running;
	timeAgo = GetTime();//记录时间
}

//按下电脑解题（c）的处理函数表示进入电脑解题
void ComputerPlayKeyPress()
{
	if (gameState == Running)//如果正处于人工解题，询问是否要放弃游戏
	{
		gameState = Pause;
		if (QuestionMessageShowFixPosition("放弃当前局？    ","键入y表示是，任意键表示否",red) == 0)
		{
			gameState = Running;
			return;
		}
	}
	gameState = ComputerPlay;
	runningTime = 0;
	ComputerPlayInitialize(pad,padConstFlag);//电脑解题初始化
	DrawNumbers(pad,padAnswer,padConstFlag);//画数字（因为全为0，所以实质上是在清楚界面上的数字）
	DrawTime(gameState,0);//显示初始时间
	CursorGotoXYReal(45,1);
	SetColor(blue);
	printf("按回车键开始电脑求解");
	CursorGotoXY(cursorX,cursorY);//将光标归位
}

//当处于电脑解题状态，按下回车键触发表示开始解题
//电脑开始解题，并且显示出来
void ComputerPlayEnterPress()
{
	gameState = ComputerPlayEnd;
	runningTime = ComputerPlaySodoku(pad,padConstFlag,padAnswer);
	ShowComputerPlayAnswer(padConstFlag,padAnswer,gameState,runningTime);
}

//按下暂定（p）触发
void PauseKeyPress()
{
	if (gameState == Running)
	{
		gameState = Pause;
		CursorGotoXYReal(54,1);
		SetColor(blue);
		printf("暂停");
	}
	else if (gameState == Pause)
	{
		gameState = Running;
		CursorGotoXYReal(54,1);
		printf("    ");
	}
	CursorGotoXY(cursorX,cursorY);
}

//按下删除键时，删除玩家自己写的数字
void DeletePress()
{
	if (padConstFlag[cursorY][cursorX] == 0 && gameState == Running)
	{
		pad[cursorY][cursorX] = 0;
		WriteCharAt(cursorX,cursorY,' ');
	}
	else if (gameState == ComputerPlay)
	{
		pad[cursorY][cursorX] = 0;
		padConstFlag[cursorY][cursorX] = 0;
		WriteCharAt(cursorX,cursorY,' ');
	}
	else if (gameState == ComputerPlayEnd )
		return;
}

//检测新时间和旧时间相比，有没有超过1秒钟。有的话用时+1秒并显示
void TimeCheck()
{
	if (gameState == Running)
	{
		timeNow = GetTime();//记录新时间
		if (timeNow - timeAgo >= 1)
		{
			timeAgo = timeNow;
			runningTime++;
			DrawTime(gameState,runningTime);
		}
	}
	else if (gameState == Pause)
	{
		timeNow = GetTime();
		timeAgo = timeNow;
	}
}

//检测数字与字母按键
void KeyCheck(char key)
{
	switch (key)
	{
	//case 'P':{PauseKeyPress();}break;
	case 'p'://pause
		{
			PauseKeyPress();
		}break;
	//case 'A':{}
	case 'a'://answer
		{
			AnswerKeyPress();
		}break;
	//case 'R':{}
	case 'r'://restart
		{
			RestartKeyPress();
		}break;
	//case 'C':{}
	case 'c'://computerPlay
		{
			ComputerPlayKeyPress();
		}break;
	default:
		{
			if (key >= '1' && key <= '9')
				NumberKeyPress(key);
		}break;
	}
	
}

//检测方向键并作出反应
void DirectionKeyCheck(char key)
{
	switch (key)
	{
	case UP://向上方向键
		{
			if (cursorY-1 >= 0)//如果没有往上走不会越界的话便将光标向上移动一格
				CursorGotoXY(cursorX,cursorY-1);
		}break;
	case DOWN:
		{
			if (cursorY+1 < 9)
				CursorGotoXY(cursorX,cursorY+1);
		}break;
	case LEFT:
		{
			if (cursorX-1 >= 0)
				CursorGotoXY(cursorX-1,cursorY);
		}break;
	case RIGHT:
		{
			if (cursorX+1 < 9)
				CursorGotoXY(cursorX+1,cursorY);
		}break;
	case VK_ESCAPE://退出键即键盘上的Esc
		{
			exit(0);
		}break;
	case VK_RETURN://回车键，用来确定开始电脑解题
		{
			if (gameState == ComputerPlay)//如果现在处于电脑阶解题，那么开始
			{
				gameState = ComputerPlayEnd;
				ComputerPlayEnterPress();//处理函数
			}
		}break;
	case VK_BACK://删除键
		{
			DeletePress();
		}break;
	}
	
}

//用于检测键盘的线程处理函数。参数dummy调用时不用写。也不知道什么意思。
void KeyCheckThread(void *dummy)
{
	char keyType;
	while(1)
	{
		keyType = getch();//获取按键
		DirectionKeyCheck(keyType);//sudokuGameLogic.h中的函数，判断是否是方向按键
		KeyCheck(keyType);//sudokuGameLogic.h中的函数，判断是否是数字键或者字母键
		keyType = getch();//再获得一次。因为方向键需要获取两次。而为了不影响数字键和字母键，所以在下面两个判断函数都运行一下
		KeyCheck(keyType);
		DirectionKeyCheck(keyType);
	}
}

//游戏循环开始主函数
void GameRun()
{
	Sleep(10);//程序暂停10ms。主要是防止程序运行过快
	TimeCheck();//sudokuGameLogic.h中的函数，检测新时间和旧时间相比，有没有超过1秒钟。有的话用时+1秒并显示
}