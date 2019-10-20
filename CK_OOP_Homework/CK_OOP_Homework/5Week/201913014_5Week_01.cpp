#include <iostream>
#include <random>
using namespace std;

/*
	define
	�� = row
	�� = column

	n  ��n�� n��n+1��
	n+1��n�� ...
*/

class Matrix
{
	const int column;
	const int row;
	int** data;

public:
	Matrix(const int&row, const int& col) : column(col), row(row)
	{
		data = new int*[row];
		for (int i = 0; i < row; ++i)
		{
			data[i] = new int[col];
		}
	}

	void SetRandom()
	{
		mt19937 mt((random_device())());
		uniform_int<> range(0, 9);
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < column; ++j)
			{
				data[i][j] = range(mt);
			}
		}
	}

	void Print()
	{
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < column; ++j)
			{
				cout << data[i][j] << '\t';
			}
			cout << endl;
		}
	}
};

int main()
{
	int c, r;
	cout << "�� �Է� : ";
	cin >> c;
	cout << "�� �Է� : ";
	cin >> r;

	Matrix mat(c, r);
	mat.SetRandom();
	mat.Print();
}