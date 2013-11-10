#include "sudokuControl.h"
#include "sudokuLogic.h"

enum GameState {Running, AnswerShow,Win, ComputerPlay, ComputerPlayEnd,Pause};

//将pad[][]中的数字填到格子里
void DrawNumbers(int pad[][9],int padAnswer[][9],int padConstFlag[][9])
{
	int i,j;
	SetColor(gray);//设置灰色
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (padConstFlag[i][j] != 0)//如果这个值是不可修改的
			{
				WriteNumberAt(j,i,pad[i][j]);
			}
			else
			{
				WriteCharAt(j,i,' ');//填写个空格
			}

		}
	}
	CursorGotoXY(0,0);//恢复坐标到(0,0)
}

//初始化游戏并且填写数字
void InitialAndDrawGame(int pad[][9],int padAnswer[][9],int padConstFlag[][9])
{
	int i,j;
	SetColor(gray);
	GameInitialize(pad,padAnswer,padConstFlag);
	DrawNumbers(pad,padAnswer,padConstFlag);
}

//画游戏界面，包括格子，外框，帮助，菜单，标题
void DrawPad()
{
	int i,j;
	SetColor(black);
	LineWithoutOffset(0,0,0,23,'#');
	LineWithoutOffset(78,0,78,23,'#');
	LineWithoutOffset(0,0,78,0,'#');
	LineWithoutOffset(0,23,78,23,'#');


	SetColor(darkCryn);//设为青色
	for (i = 0; i < 9; i++)//画竖细线
	{
		Line(4*i,0,4*i,18,'|');
	}
	for (i = 0; i < 9; i++)//画横细线
	{
		Line(0,2*i,36,2*i,'-');	
	}

	SetColor(black);//设为黑色
	Line(0,0,36,0,'*');//画横向的粗线
	Line(0,6,36,6,'*');
	Line(0,6*2,36,6*2,'*');
	Line(0,6*3,36,6*3,'*');

	Line(0,0,0,18,'*');//画竖向的粗线
	Line(12,0,12,18,'*');
	Line(12*2,0,12*2,18,'*');
	Line(12*3,0,12*3,18,'*');
	
	//标题
	SetColor(blue);
	CursorGotoXYRealWithoutOffest(31,0);
	printf("数    SUDOKU    独");
	SetColor(red);
	CursorGotoXYRealWithoutOffest(66,0);
	printf("[Esc]:退出");

	//帮助和菜单
	SetColor(gray);
	CursorGotoXYReal(45,10);
	printf("↑↓←→：   移动光标");
	CursorGotoXYReal(45,12);
	printf("数字键 :     填写数字");
	CursorGotoXYReal(45,14);
	printf("Backspace :  删除数字");
	SetColor(darkGreen);
	CursorGotoXYReal(45,16);
	printf("[P] :        暂停游戏");
	CursorGotoXYReal(45,17);
	printf("[A] :        显示答案");
	CursorGotoXYReal(45,18);
	printf("[R] :        新开数独");
	CursorGotoXYReal(45,19);
	printf("[C] :        电脑解答");
}

//画时间
void DrawTime(GameState gameState, int runningTime)
{
	SetColor(black);//设置为黑色
	CursorGotoXYReal(48,4);
	if (gameState == Running)//如果是人工解题中,则显示时分秒的解题时间
	{
		printf("用时：%d时%d分%d秒    ",runningTime/3600,runningTime/60%60,runningTime%60);
	}
	else if (gameState == ComputerPlay || gameState == ComputerPlayEnd)//如果是机器解题则显示毫秒的机器用时
	{
		printf("用时：%.3lf毫秒     ",(double)runningTime/1000);
	}
	CursorGotoXY(cursorX,cursorY);
}

//显示答案
void ShowAnswer(int pad[][9],int padConstFlag[][9],int padAnswer[][9])
{
	SetColor(red);//设为红色
	CursorGotoXYReal(50,1);
	printf("   数独答案      ");//写几个字
	int i,j;
	for (i = 0; i < 9; i++)//将那些没有填出来的数字显示出来
	{
		for (j = 0; j < 9; j++)
		{
			if (padConstFlag[i][j] == 0)
			{
				if (pad[i][j] == 0)
				{
					WriteNumberAt(j,i,padAnswer[i][j]);
				}
			}
		}
	}
}

//显示电脑解题的结果
void ShowComputerPlayAnswer(int padConstFlag[][9], int padAnswer[][9],GameState gameState,int  runningTime)
{
	int i,j;
	SetColor(blue);//用蓝色来写
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (padConstFlag[i][j] == 0)
			{
				WriteNumberAt(j,i,padAnswer[i][j]);
			}
		}
	}
	DrawTime(gameState,runningTime);//并且把用时画出来
}

