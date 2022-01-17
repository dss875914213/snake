#include "Snake.h"

SnakeGame::SnakeGame()
	:m_hOut(NULL),
	m_dir(0),
	m_begin(false),
	m_eatFood(false),
	m_speed(200),
	m_foodX(0),
	m_foodY(0),
	m_score(0),
	e(time(0)),
	ux(2, width-2),
	uy(1, height-1)
{
	// 开始游戏
	HideCursor();
	
	m_begin = true;
	Snake head, tail;
	while (1)
	{
		if (m_begin)
		{
			PrintMap();
			m_dir = _getch();
			InitSnake();
			CreateFood();
			m_begin = m_eatFood = false;
		}
		tail = m_snake.back();
		head = m_snake.front();
		// 前进一步，尾部缩短
		
		if (m_eatFood)
		{
			tail.image = '+';
			m_eatFood = false;
		}
		else
		{
			m_snake.pop_back();
			// 将画面中的尾部置为空
			ClearSnake(tail);
		}
		// s 代表向下  a 代表向左 d 代表向右 w 代表向上
		if (m_dir == 's')
			++head.y;
		else if (m_dir == 'a')
			--head.x;
		else if (m_dir == 'd')
			++head.x;
		else if(m_dir == 'w')
			--head.y;
		try
		{
			JudgeCrash();
		}
		catch (runtime_error& quitSignal)
		{
			throw quitSignal;
		}
		// 将上一次的头的 image 改为 +
		m_snake.front().image = '+';
		m_snake.push_front(head);

		// 绘制改变的两个点
		PrintSnake();
		Sleep(m_speed + 30);
		if (head.x == m_foodX && head.y == m_foodY)
			FoodEaten();
		// 检测是否有键盘输入
		if (_kbhit())
			UserInput();
	}

}

// 打印地图
void SnakeGame::PrintMap()
{
	int i;
	// 最上面那行
	for (i = 0; i != width; i += 2)
		cout << "■"; // 宽度占2，高度占1
	// 最左边那行，靠 endl 换行
	GotoXY(0, 1);
	for (i = 1; i != height; i++)
		cout << "■" << endl;
	// 最右边那行
	for(i=1;i!=height;i++)
	{
		GotoXY(width - 2, i);
		cout << "■";
	}
	// 最下面那行
	GotoXY(0, height - 1);
	for (i = 0; i != width; i += 2)
		cout << "■";
	cout << "贪吃蛇：1.方向键开始游戏 2.*代表食物 3.空格键代表暂停游戏\n";
}

void SnakeGame::GotoXY(short x, short y)
{
	m_hOut = GetStdHandle(STD_OUTPUT_HANDLE); // 获得句柄
	m_pos = { x, y };
	SetConsoleCursorPosition(m_hOut, m_pos); // 移动光标
}

// 隐藏光标
void SnakeGame::HideCursor()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(handle, &cursorInfo); // 获得控制台光标信息
	cursorInfo.bVisible = false; // 控制控制台光标
	SetConsoleCursorInfo(handle, &cursorInfo); // 设置控制台光标状态
}

// 初始化蛇，可以自行改变
void SnakeGame::InitSnake()
{
	m_snake.push_front({'@', width/2, height/2});
	for (int i = 0; i < 2; i++)
		m_snake.push_back({ '+', width / 2, static_cast<short>(height / 2 + i + 1) });
}

// 判断是否食物产生位置和蛇身冲突
int SnakeGame::WrongLocation()
{
	for (Snake body : m_snake)
		if (body.x == m_foodX && body.y == m_foodY)
			return 0;
	return 1;
}

void SnakeGame::CreateFood()
{
	do 
	{
		m_foodX = ux(e);
		m_foodY = uy(e);
	} while (!WrongLocation()); // 处理冲突
	GotoXY(m_foodX, m_foodY);
	// 打印食物
	cout << "*" << endl;
}

// 打印蛇身
void SnakeGame::PrintSnake()
{
	// 只绘制前进的那一点和原来头的位置（因为头的 image 和身体的 image 不一样）
	for (auto iter = m_snake.cbegin(); iter <= m_snake.cbegin() + 1 && iter < m_snake.cend(); ++iter)
	{
		GotoXY(iter->x, iter->y);
		cout << iter->image;
	}
}

void SnakeGame::ClearSnake(Snake& tail)
{
	GotoXY(tail.x, tail.y);
	cout << ' '; // 覆盖蛇尾，不适用清屏函数
}

// 判断是否撞墙或吃到自己
void SnakeGame::JudgeCrash()
{
	int bCollide = 0;
	if (m_snake.size() > 5)
	{
		// 头是否和身体其他部分相撞
		for (auto iter = m_snake.begin() + 1; iter != m_snake.end(); iter++)
			if (iter->x == m_snake.begin()->x&& iter->y == m_snake.begin()->y)
				bCollide = 1;
	}

	// 检测是否撞墙或者吃到自身
	if (bCollide || m_snake.front().x == 1 || m_snake.front().x == width - 2 ||
		m_snake.front().y == 0 || m_snake.front().y == height - 1) 
	{
		GotoXY(width / 2 - 10, height / 2);
		cout << "游戏结束！ 您的分数是: " << m_score << "分(回车继续)" << endl;
		while (1)
		{
			// 从控制台读取一个字符，但不显示在屏幕上
			m_dir = _getch();
			if (m_dir == '\r')
				break;
		}
		runtime_error quit("游戏结束，正常退出");
		throw quit;
	}
}

// 吃到食物
void SnakeGame::FoodEaten()
{
	CreateFood();
	m_eatFood = true;
	m_speed *= 0.8;
	++m_score;
}

void SnakeGame::UserInput()
{
	char ch;
	switch (ch = _getch())
	{
	case 'w':
		if (m_dir != 's')
			m_dir = ch;
		break;
	case 'a':
		if (m_dir != 'd')
			m_dir = ch;
		break;
	case 's':
		if (m_dir != 'w')
			m_dir = ch;
		break;
	case 'd':
		if (m_dir != 'a')
			m_dir = ch;
		break;
	case 'v':
		m_speed *= 0.8;
		break;
	case 'b':
		m_speed *= 1.5;
		break;
	case ' ':
		GotoXY(width / 2, height);
		cout << "游戏已暂停，任意键继续";
		_getch();
		GotoXY(width / 2, height);
		cout << "                     ";
		break;
	default:
		break;
	}
}
