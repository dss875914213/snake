#include "Snake.h"

int main()
{
	// ���ñ���
	system("title ̰����");
	// ���ڿ�ȸ߶�
	system("mode con cols=120 lines=42");
	try
	{
		SnakeGame game;
	}
	catch (runtime_error& gameEnd)
	{
		// �����Ļ
		system("CLS");
		cout << gameEnd.what();
		_getch();
	}
}


