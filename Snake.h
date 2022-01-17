#include <iostream>
#include <windows.h>
#include <conio.h>
#include <deque>
#include <ctime>
#include <stdexcept>
#include <random>
using namespace  std;

// 蛇类结构体
struct Snake
{
	// 该位置用什么符号表示
	char image;
	short x, y; // 坐标
};

class SnakeGame
{
public:
	SnakeGame();
	void PrintMap();
	void GotoXY(short x, short y);
	void HideCursor();
	void InitSnake();
	int WrongLocation();
	void CreateFood();
	void PrintSnake();
	void ClearSnake(Snake& tail);
	void JudgeCrash();
	void FoodEaten();
	void UserInput();

private:
	enum MapSize { width = 120, height = 40 }; // 地图尺寸
	HANDLE m_hOut;
	COORD m_pos;
	char m_dir;
	bool m_begin;
	bool m_eatFood;
	double m_speed;
	deque<Snake> m_snake;
	int m_foodX, m_foodY;
	int m_score;
	std::default_random_engine e;
	std::uniform_int_distribution<unsigned> ux;
	std::uniform_int_distribution<unsigned> uy;
};