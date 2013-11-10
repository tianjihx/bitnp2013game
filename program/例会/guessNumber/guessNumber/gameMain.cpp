#include <Windows.h>
#include <iostream>
#include <time.h>
#include <conio.h>
using namespace std;


int main()
{
	void SetPurple();
	void SetWhite();
	int theNumber;
	int bound;
	int lastTimes = 5;
	srand(time(0));
	SetPurple();
	cout << "你要猜多大以内的数字？";
	SetWhite();
	cin >> bound;
	theNumber = rand() % bound;
	int guessNumber;
	cout << "请输入一个小于"<< bound <<"的数字";
	while (1)
	{
		if (lastTimes  > 0)
			lastTimes--;
		else
		{
			cout << "gameover!!!!" << endl;
			break;
		}
		SetWhite();
		cin >> guessNumber;
		SetPurple();
		if (guessNumber == theNumber)
		{
			cout << "你猜对了！\n";
			break;
		}
		else if (guessNumber < theNumber)
		{
			cout << "猜小了\n";
		}
		else if (guessNumber > theNumber)
		{
			cout << "猜大了\n";
		}
		cout << "剩余猜的次数为：" << lastTimes <<endl;
	}
	return 0;
}


void SetPurple()
{
	WORD color;
	color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		color);
}

void SetWhite()
{
	WORD color;
	color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_GREEN;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		color);
}