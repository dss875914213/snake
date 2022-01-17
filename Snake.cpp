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
	// ��ʼ��Ϸ
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
		// ǰ��һ����β������
		
		if (m_eatFood)
		{
			tail.image = '+';
			m_eatFood = false;
		}
		else
		{
			m_snake.pop_back();
			// �������е�β����Ϊ��
			ClearSnake(tail);
		}
		// s ��������  a �������� d �������� w ��������
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
		// ����һ�ε�ͷ�� image ��Ϊ +
		m_snake.front().image = '+';
		m_snake.push_front(head);

		// ���Ƹı��������
		PrintSnake();
		Sleep(m_speed + 30);
		if (head.x == m_foodX && head.y == m_foodY)
			FoodEaten();
		// ����Ƿ��м�������
		if (_kbhit())
			UserInput();
	}

}

// ��ӡ��ͼ
void SnakeGame::PrintMap()
{
	int i;
	// ����������
	for (i = 0; i != width; i += 2)
		cout << "��"; // ���ռ2���߶�ռ1
	// ��������У��� endl ����
	GotoXY(0, 1);
	for (i = 1; i != height; i++)
		cout << "��" << endl;
	// ���ұ�����
	for(i=1;i!=height;i++)
	{
		GotoXY(width - 2, i);
		cout << "��";
	}
	// ����������
	GotoXY(0, height - 1);
	for (i = 0; i != width; i += 2)
		cout << "��";
	cout << "̰���ߣ�1.�������ʼ��Ϸ 2.*����ʳ�� 3.�ո��������ͣ��Ϸ\n";
}

void SnakeGame::GotoXY(short x, short y)
{
	m_hOut = GetStdHandle(STD_OUTPUT_HANDLE); // ��þ��
	m_pos = { x, y };
	SetConsoleCursorPosition(m_hOut, m_pos); // �ƶ����
}

// ���ع��
void SnakeGame::HideCursor()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(handle, &cursorInfo); // ��ÿ���̨�����Ϣ
	cursorInfo.bVisible = false; // ���ƿ���̨���
	SetConsoleCursorInfo(handle, &cursorInfo); // ���ÿ���̨���״̬
}

// ��ʼ���ߣ��������иı�
void SnakeGame::InitSnake()
{
	m_snake.push_front({'@', width/2, height/2});
	for (int i = 0; i < 2; i++)
		m_snake.push_back({ '+', width / 2, static_cast<short>(height / 2 + i + 1) });
}

// �ж��Ƿ�ʳ�����λ�ú������ͻ
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
	} while (!WrongLocation()); // �����ͻ
	GotoXY(m_foodX, m_foodY);
	// ��ӡʳ��
	cout << "*" << endl;
}

// ��ӡ����
void SnakeGame::PrintSnake()
{
	// ֻ����ǰ������һ���ԭ��ͷ��λ�ã���Ϊͷ�� image ������� image ��һ����
	for (auto iter = m_snake.cbegin(); iter <= m_snake.cbegin() + 1 && iter < m_snake.cend(); ++iter)
	{
		GotoXY(iter->x, iter->y);
		cout << iter->image;
	}
}

void SnakeGame::ClearSnake(Snake& tail)
{
	GotoXY(tail.x, tail.y);
	cout << ' '; // ������β����������������
}

// �ж��Ƿ�ײǽ��Ե��Լ�
void SnakeGame::JudgeCrash()
{
	int bCollide = 0;
	if (m_snake.size() > 5)
	{
		// ͷ�Ƿ����������������ײ
		for (auto iter = m_snake.begin() + 1; iter != m_snake.end(); iter++)
			if (iter->x == m_snake.begin()->x&& iter->y == m_snake.begin()->y)
				bCollide = 1;
	}

	// ����Ƿ�ײǽ���߳Ե�����
	if (bCollide || m_snake.front().x == 1 || m_snake.front().x == width - 2 ||
		m_snake.front().y == 0 || m_snake.front().y == height - 1) 
	{
		GotoXY(width / 2 - 10, height / 2);
		cout << "��Ϸ������ ���ķ�����: " << m_score << "��(�س�����)" << endl;
		while (1)
		{
			// �ӿ���̨��ȡһ���ַ���������ʾ����Ļ��
			m_dir = _getch();
			if (m_dir == '\r')
				break;
		}
		runtime_error quit("��Ϸ�����������˳�");
		throw quit;
	}
}

// �Ե�ʳ��
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
		cout << "��Ϸ����ͣ�����������";
		_getch();
		GotoXY(width / 2, height);
		cout << "                     ";
		break;
	default:
		break;
	}
}
