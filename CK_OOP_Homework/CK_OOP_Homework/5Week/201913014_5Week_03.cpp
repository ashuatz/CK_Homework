#include <iostream>
using namespace std;

class Dog
{
private:
	char* name;
	int age;
	int weight;
public:
	Dog(const char* name, int age, int weight) : age(age), weight(weight)
	{
		int len = strlen(name) + 1;
		this->name = new char[len];
		strcpy_s(this->name, len, name);
	}

	Dog(const Dog& copy) : age(copy.age), weight(copy.weight)
	{
		int len = strlen(copy.name) + 1;
		this->name = new char[len];
		strcpy_s(this->name, len, copy.name);
	}

	void ShowData()const
	{
		cout << "이름 : " << name << endl;
		cout << "나이 : " << age << endl;
		cout << "무게 : " << weight << endl;
	}

	~Dog()
	{
		delete[] name;
	}
};

int main()
{
	Dog obj1("happy", 3, 4);
	Dog obj2 = obj1;
	obj1.ShowData();
	obj2.ShowData();
	return 0;
}