#include <iostream>
#include <chrono>
#include <conio.h>

constexpr auto VK = 224;
constexpr auto VK_UP = 72;
constexpr auto VK_LEFT = 75;
constexpr auto VK_RIGHT = 77;
constexpr auto VK_DOWN = 80;
constexpr auto SPACE = ' ';

using namespace std;

auto min = [](const auto& a, const auto& b) { return (a < b) ? a : b; };

template<typename T>
const T& clamp(const T& target, const T& min, const T& max)
{
	return target > max ? max : target < min ? min : target;
}

enum class CellType
{
	None,
	Black,
	White,
};

struct Point
{
	int x;
	int y;

	bool isOver(const int& min, const int& max) const
	{
		return  min > x || x >= max || min > y || y >= max;
	}

	Point& operator+= (const Point& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Point operator-= (const Point& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}
};

class Cell
{
private:
	Point pos;
	CellType myType;

public:
	void Initialize(const Point& pos)
	{
		this->pos = pos;
		this->myType = CellType::None;
	}

	bool TryChangeOwner(const CellType& target)
	{
		if (myType != CellType::None)
			return false;

		myType = target;
		return true;
	}

	const CellType GetMyType() const { return myType; }
};

class Board
{
private:
	static constexpr int size = 10;
	Cell** cells;
	Point cursorPos;

	const Point SearchDirection[4]{ Point{1,1}, Point{1,0}, Point{1,-1}, Point{0,1} };
public:
	void Init()
	{
		cursorPos.x = size / 2;
		cursorPos.y = size / 2;

		cells = new Cell*[size];
		for (int i = 0; i < size; ++i)
		{
			cells[i] = new Cell[size];
			for (int j = 0; j < size; ++j)
			{
				cells[i][j].Initialize(Point{ j, i });
			}
		}
	}

	void PrintAll(bool withCursor = true) const
	{
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				if (withCursor && cursorPos.x == x && cursorPos.y == y)
				{
					cout << "¡Ý";
					continue;
				}

				const auto&& type = cells[y][x].GetMyType();
				switch (type)
				{
					case CellType::None: cout << "¦« "; break;
					case CellType::Black: cout << "¡Û"; break;
					case CellType::White: cout << "¡Ü"; break;
					default:
						break;
				}
			}
			cout << endl;
		}
	}

	const int SearchLine(const CellType& type, const Point& dir) const
	{
		int cnt = -1;
		for (Point pos = cursorPos; !pos.isOver(0, size) && cells[pos.y][pos.x].GetMyType() == type; pos += dir, cnt++);
		for (Point pos = cursorPos; !pos.isOver(0, size) && cells[pos.y][pos.x].GetMyType() == type; pos -= dir, cnt++);
		return cnt;
	}

	bool CheckGameEnd(const CellType& type) const
	{
		for (const auto& dir : SearchDirection)
		{
			if (SearchLine(type, dir) == 5)
				return true;
		}

		return false;
	}

	bool SetCell(const CellType& target) const
	{
		return cells[cursorPos.y][cursorPos.x].TryChangeOwner(target);
	}

	void MoveCursor(const Point& delta)
	{
		cursorPos.x = clamp(cursorPos.x + delta.x, 0, size - 1);
		cursorPos.y = clamp(cursorPos.y + delta.y, 0, size - 1);
	}

	~Board()
	{
		for (int i = 0; i < size; ++i)
		{
			delete[] cells[i];
		}
		delete[] cells;
	}
};

int main()
{
	Board board;
	bool isBlackTurn = true;

	board.Init();
	while (true)
	{
		//clear board
		system("cls");

		//render board
		board.PrintAll();
		cout << "Turn : " << (isBlackTurn ? "Black" : "White") << endl;
		cout << "Move ¡ç ¡æ ¡è ¡é And Select <Space> " << endl;

		//wait input
		while (!_kbhit());

		//input and processing
		int code = _getch();
		if (code == VK)
			code = _getch(); // for vk

		bool isPass = false;
		switch (code)
		{
			case VK_DOWN:	board.MoveCursor(Point{ 0,1 }); break;
			case VK_UP:		board.MoveCursor(Point{ 0,-1 }); break;
			case VK_LEFT:	board.MoveCursor(Point{ -1,0 }); break;
			case VK_RIGHT:	board.MoveCursor(Point{ 1,0 }); break;
			case SPACE:		isPass = board.SetCell(isBlackTurn ? CellType::Black : CellType::White); break;
			default: break;
		}

		if (!isPass)
			continue;

		//Check Game is End
		if (board.CheckGameEnd(isBlackTurn ? CellType::Black : CellType::White))
			break;

		//Change Turn
		isBlackTurn = !isBlackTurn;
	}

	system("cls");
	board.PrintAll(false);
	cout << (isBlackTurn ? "Black" : "White") << " Win" << endl;
}