#include <stdio.h>
#include "windows.h"
#include <math.h>
#include "conio.h"
#include "time.h"

extern int cursorX;
extern int cursorY;

//定义了两个固定的便宜量，即是坐标轴平移的量
int const offsetX = 5, offsetY = 2;

//定义了一个颜色集合
enum Color {red,green,blue,cryn,magenta,yellow,white,black,darkCryn,darkGreen,gray};


//设定光标在新坐标系下的坐标
void CursorGotoXY(int x, int y)
{
	//Initialize the coordinates
	COORD coord = {x*4+offsetX+2, y*2+offsetY+1};//设置一个坐标，平移原点后，将（x,y）通过x*4+offsetX+2和y*2+offsetY+1变换为新坐标系下的坐标
	//Set the position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);//一个设定光标的位置的函数

	cursorX = x;//将光标的位置保存下来
	cursorY = y;
	
}

//设定光标在平移后的原始坐标系下的坐标
void CursorGotoXYReal(int x, int y)
{
	//Initialize the coordinates
	COORD coord = {x+offsetX, y+offsetY};//没有变换坐标，只是将坐标原点平移
	//Set the position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//设定光标在未平移原始坐标系下的坐标
void CursorGotoXYRealWithoutOffest(int x, int y)
{
	//Initialize the coordinates
	COORD coord = {x, y};//没有变换坐标，只是将坐标原点平移
	//Set the position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//设定字体颜色
//参数color集合Color中的一个
void SetColor(Color color)
{
	WORD colorA;//定义一个WORD类型的变量来存颜色的代码，这是规定的格式
	//colorA的值可以是下面几种值的一种或者几种，前景色就是文字的颜色
	//FOREGROUND_RED：前景色_红色
	//FOREGROUND_BLUE：前景色_蓝色
	//FOREGROUND_GREEN：前景色_绿色
	//FOREGROUND_INTENSITY:单独使用时灰色，与颜色一同使用是加亮颜色
	//黑色是 0 
	//这几种参数可以用 | 来搭配使用产生不同颜色，排列组合共可以产生可以组成n种颜色（组合数学没学好……），下面便是自行定义了几种颜色
	switch (color)
	{
	case black:{colorA = 0;}break;//黑色
	case red:{colorA = FOREGROUND_INTENSITY | FOREGROUND_RED;}break;//亮红色
	case blue:{colorA = FOREGROUND_INTENSITY |FOREGROUND_BLUE;}break;//亮蓝色
	case green:{colorA = FOREGROUND_INTENSITY |FOREGROUND_GREEN;}break;//亮绿色
	case darkGreen:{colorA = FOREGROUND_GREEN;}break;//绿色
	case cryn:{colorA = FOREGROUND_INTENSITY |FOREGROUND_BLUE | FOREGROUND_GREEN;}break;//青色
	case darkCryn:{colorA = FOREGROUND_BLUE | FOREGROUND_GREEN;}break;
	case magenta:{colorA = FOREGROUND_INTENSITY |FOREGROUND_BLUE| FOREGROUND_RED;}break;//品红
	case yellow:{colorA = FOREGROUND_INTENSITY | FOREGROUND_RED| FOREGROUND_GREEN;}break;//亮黄色
	case white:{colorA = FOREGROUND_RED| FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;}break;//红绿蓝再加强为白色
	case gray:{colorA = FOREGROUND_INTENSITY;}break;
	default:colorA = FOREGROUND_INTENSITY;
			break;
	}
	//前面的BACKGROUND_XXX是背景色，即文字的底色。这里将背景色设为白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY|BACKGROUND_BLUE| BACKGROUND_GREEN|BACKGROUND_RED | colorA);
}

//write the (int)number at position(x,y).x+ to right, y+ to down（新坐标系中）
void WriteNumberAt( int x, int y,int number)
{
	CursorGotoXY(x,y);
	printf("%d",number);
}

//在新坐标系中的(x,y)坐标下写字符c
void WriteCharAt( int x, int y,char c)
{
	CursorGotoXY(x,y);
	printf("%c",c);
}

//用字符c画条直线，直线从平移后坐标系下(x1,y1)到(x2,y2)。DDA算法——参考《计算机图形学》书上
void Line(int x1, int y1, int x2, int y2, char c)
{
	double dx,dy,x,y,length;
	int i;
	if (abs(x2-x1) > abs(y2-y1))
		length = abs(x2-x1);
	else
		length = abs(y2-y1);
	dx = (x2-x1) / length;
	dy = (y2-y1) / length;
	x =x1;y = y1;
	for (i = 0; i <= length; i++)
	{
		CursorGotoXYReal(x,y);
		printf("%c",c);
		x += dx;
		y += dy;
	}

}

//用字符c画条直线，直线从未平移坐标系下(x1,y1)到(x2,y2)。DDA算法——参考《计算机图形学》书上
void LineWithoutOffset(int x1, int y1, int x2, int y2, char c)
{
	double dx,dy,x,y,length;
	int i;
	if (abs(x2-x1) > abs(y2-y1))
		length = abs(x2-x1);
	else
		length = abs(y2-y1);
	dx = (x2-x1) / length;
	dy = (y2-y1) / length;
	x =x1;y = y1;
	for (i = 0; i <= length; i++)
	{
		CursorGotoXYRealWithoutOffest(x,y);
		printf("%c",c);
		x += dx;
		y += dy;
	}

}

//获取系统当前时间，返回的是自1970年1月1日以来到现在所经过的秒数.1970年是系统规定的
int GetTime()
{
	time_t t;//定义时间格式的变量t，其实就是个整形
	time(&t);//获取当前时间，赋给t
	return t;//隐式转换为int型返回
}

//在固定位置显示一个两行的询问语句。s1为第一行要显示的字符串，s2为第二行要显示的字符串。color为要显示的颜色
int QuestionMessageShowFixPosition(char s1[],char s2[], Color color)
{
	int state;//用来存储状态
	SetColor(color);
	CursorGotoXYReal(50,1);
	printf(s1);
	CursorGotoXYReal(45,2);
	printf(s2);
	char yesno = getch();//读取一个字符
	if (yesno != 'y' && yesno != 'Y')//如果敲入的不是y，表示否
	{
		state = 0;
	}
	else
		state = 1;
	CursorGotoXYReal(50,1);//这几句话用来将刚才显示的话消除掉
	printf("                       ");
	CursorGotoXYReal(45,2);
	printf("                            ");
	return state;
}

