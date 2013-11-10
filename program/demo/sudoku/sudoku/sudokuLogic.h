#include <stdlib.h>
#include <time.h>
#include <Windows.h>

//检测number在当前列有没有重复的
int CheckNumberExistInColum(int pad[][9], int colum, int number)
{
	int iRow,i;
	for (iRow = 0; iRow < 9; iRow++)
	{
		if (number == pad[iRow][colum])
		{
			return 1;
		}
	}
	return 0;
}

//检测number在当前行有没有重复的
int CheckNumberExistInRow(int pad[][9],int row, int number)
{
	int iColum,i;
	for (iColum = 0; iColum < 9; iColum++)
	{
		if (number == pad[row][iColum])
		{
			return 1;
		}
	}
	return 0;
}

//检测number在当前宫格有没有重复的
//小宫格的编号BlockCode
//从左往右，从上往下依次编号为0,1,2,3,4,5,6,7,8
int CheckNumberExistInBlock(int pad[][9],int BlockCode,int number)
{
	int i,j;
	int iBlock = BlockCode / 3,
		jBlock = BlockCode % 3;
	for (i = iBlock * 3; i < iBlock * 3 + 3; i++)
	{
		for (j = jBlock * 3; j < jBlock * 3 + 3; j++)
		{
			if (number == pad[i][j])
			{
				return 1;
			}
		}
	}
	return 0;
}

//递归求解，将结果放到pad[][]中
int TryAndPut(int pad[][9],int i, int j)
{
	int numberTry;
	for (numberTry = 1; numberTry <= 9; numberTry++)
	{
		if (i > 8 || j > 8)
		{
			return 1;
		}
		int BlockCode = (i/3)*3+(j/3);//将数独数组的坐标(iPad, jPad)变换成小宫格的编号BlockCode.
		if (!CheckNumberExistInBlock(pad,BlockCode, numberTry) &&
			!CheckNumberExistInColum(pad,j,numberTry) &&
			!CheckNumberExistInRow(pad,i,numberTry) )
		{
			pad[i][j] = numberTry;
			if (j < 8)
			{
				if (TryAndPut(pad,i,j+1))
				{
					return 1;
				}
			}
			else
			{
				if (i < 8)
				{
					if (TryAndPut(pad,i+1,0))
					{
						return 1;
					}
				}
				else
					return 1;
			}
			pad[i][j] = 0;
		}
	}
	return 0;
}

//用pad[][]填充九宫格
void CreateSodukuPad(int pad[][9])
{
	srand(time(0));
	int temp[100];
	int nineNumberCreater[100], i,j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			pad[i][j] = 0;
		}
	}
	for (i = 0; i < 9; i++)
	{
		nineNumberCreater[i] = i+1;
	}
	//将nineNumberCreater数组中1~9数字的顺序打乱
	for (i = 0; i < 9; i++)
	{
		int randPos = rand() % 9;
		int tempSwap = nineNumberCreater[i];
		nineNumberCreater[i] = nineNumberCreater[randPos];
		nineNumberCreater[randPos] = tempSwap;
	}

	//将打乱顺序的数字填到第一行中
	for (i = 0; i < 9; i++)
	{
		pad[0][i] = nineNumberCreater[i];
	}
	TryAndPut(pad,1,0);//递归将pad剩下的2到9行填满
}

//对pad[][]挖blankSize个空。将未挖的空的位置用padConstFlag记录下来。并且将挖空之前的数组用padAnswer保存下来以备显示答案之用
void SetSodokuPuzzle(int pad[][9],int padAnswer[][9],int padConstFlag[][9],int BlankSize)
{
	int i,j,k;
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
		{
			padConstFlag[i][j] = 1;
			padAnswer[i][j] = pad[i][j];
		}
	i = rand() % 9;
	j = rand() % 9;
	for (k = 0; k < BlankSize; k++)
	{
		while (pad[i][j] == 0)
		{
			i = rand() % 9;
			j = rand() % 9;
		}
		pad[i][j] = 0;
		padConstFlag[i][j] = 0;
	}
}


//初始化游戏，将对pad[][初始化，然后挖空设题
void GameInitialize(int pad[][9],int padAnswer[][9],int padConstFlag[][9])
{
	CreateSodukuPad(pad);
	int blankSize;
	/*switch (level)
	{
	case Crazy :{blankSize = 65;}break;
	case Difficult :{blankSize = 50;}break;
	case Normal :{blankSize = 35;}break;
	case Easy :{blankSize = 20;}break;
	case Relax :{blankSize = 10;}break;
	}*/
	blankSize = 30+ rand() % 3;
	SetSodokuPuzzle(pad,padAnswer,padConstFlag,30);
}

//检测是否已把空填满，即填出数独
int CheckFinished(int pad[][9])
{
	int i,j;
	for (i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			if (pad[i][j] == 0)
				return 0;
		}
	}
	return 1;
}

//电脑求解所用的递归
int TryAndPutComputerPlay(int pad[][9],int blankPosi[],int blankPosj[],int blankCount,int k)
{
	int numberTry;
	for (numberTry = 1; numberTry <= 9; numberTry++)
	{
		if (k >= blankCount)
			return 1;
		int BlockCode = (blankPosi[k]/3)*3+(blankPosj[k]/3);//将数独数组的坐标(iPad, jPad)变换成小宫格的编号BlockCode.
		if (!CheckNumberExistInBlock(pad,BlockCode, numberTry) &&
			!CheckNumberExistInColum(pad,blankPosj[k],numberTry) &&
			!CheckNumberExistInRow(pad,blankPosi[k],numberTry) )
		{
			pad[blankPosi[k]][blankPosj[k]] = numberTry;
			if (TryAndPutComputerPlay(pad,blankPosi,blankPosj,blankCount,k+1))
			{
				return 1;
			}
			pad[blankPosi[k]][blankPosj[k]] = 0;
		}
	}
	return 0;
}

//初始化电脑求解时用到的pad[][]和padConstFlag[][]。均初始化为0
void ComputerPlayInitialize(int pad[][9], int padConstFlag[][9])
{
	int i,j;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			pad[i][j] = 0;
			padConstFlag[i][j] = 0;
		}
	}
}

//电脑计算数独开始，函数返回计算的时间，单位是ms
int ComputerPlaySodoku(int pad[][9],int padConstFlag[][9], int padAnswer[][9])
{
	LARGE_INTEGER nFreq, t1,t2;
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&t1);

	int i,j,blankCount = 0;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (pad[i][j] != 0)
			{
				padConstFlag[i][j] = 1;
			}
		}
	}

	//首先扫描数独，获得未填写区域的坐标，存为一个数组blankPos[]
	int blankPosi[81],blankPosj[81];
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (pad[i][j] == 0)
			{
				blankPosi[blankCount] = i;
				blankPosj[blankCount] = j;
				blankCount++;
			}
		}
	}
	TryAndPutComputerPlay(pad,blankPosi,blankPosj,blankCount,0);
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			padAnswer[i][j] = pad[i][j];
		}
	}
	QueryPerformanceCounter(&t2);
	int dt = ((double)t2.QuadPart - (double)t1.QuadPart) * 1000000 / (double)nFreq.QuadPart;
	return dt;
}