#include "Snake.h"

int main()
{
	// 设置标题
	system("title 贪吃蛇");
	// 窗口宽度高度
	system("mode con cols=120 lines=42");
	try
	{
		SnakeGame game;
	}
	catch (runtime_error& gameEnd)
	{
		// 清除屏幕
		system("CLS");
		cout << gameEnd.what();
		_getch();
	}
}


